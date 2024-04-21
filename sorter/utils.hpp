#ifndef SORTER_UTILS_HPP
#define SORTER_UTILS_HPP

#include <tape/utils.hpp>

#include <random>

namespace ts {

static auto createRandomIntFile(const char* filename, size_t numbersAmount)
{
    auto file = createFile(filename, numbersAmount * sizeof(int32_t));

    std::random_device randD;
    std::mt19937 randMT(randD());
    std::uniform_int_distribution<int32_t> range(0, 100);

    file.seekp(0);
    for (size_t i = 0; i < numbersAmount; i++)
    {
        const int32_t val = range(randMT);
        file.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }
    return file;
}

static auto createRandomIntTape(const char* filename, size_t numbersAmount)
{
    createRandomIntFile(filename, numbersAmount);
    return Tape<int32_t>(filename);
}

template <typename T, typename EndPos = typename ITape<T>::pos_type>
void copy(ITape<T>* src, ITape<T>* dst, EndPos end = 0)
{
    using ITape<T>::MoveDirection::Forward;
    using ITape<T>::MoveDirection::Backward;

    while (true)
    {
        dst->write(src->read());
        if (src->position() == end)
        {
            break;
        }
        src->move(Backward);
        dst->move(Forward);
    }
}

template <typename T, typename Compare, typename EndPos = typename ITape<T>::pos_type>
void mergeTapeChunks(ITape<T>* src[2], EndPos end[2], ITape<T>* dst, Compare comp)
{
    using val_type = typename ITape<T>::val_type;
    using ITape<T>::MoveDirection::Forward;
    using ITape<T>::MoveDirection::Backward;

    val_type val[2] = {src[0]->read(), src[1]->read()};
    int n;
    while (true)
    {
        n = static_cast<int>(!comp(val[0], val[1]));
        dst->write(val[n]);
        dst->move(Forward);

        if (src[n]->position() == end[n])
        {
            n = 1 - n;
            break;
        }

        src[n]->move(Backward);
        val[n] = src[n]->read();
    }

    copy(src[n], dst, end[n]);
}

template <typename T, typename Compare>
size_t mergeTapes(ITape<T>* src[2], ITape<T>* dst[2], auto blockSize, Compare comp)
{
    using EndPos = typename ITape<T>::pos_type;
    using ITape<T>::MoveDirection::Forward;
    using ITape<T>::MoveDirection::Backward;

    EndPos end[2];
    size_t chunksNumber = 0;
    while (true)
    {
        end[0] = src[0]->position() <= blockSize ? 0 : src[0]->position() - blockSize + 1;
        end[1] = (src[1]->position() / blockSize) * blockSize;

        if (end[0] == 0 && end[1] == 0)
        {
            return 1;
        }

        mergeTapeChunks(src, end, dst[0], comp);
        chunksNumber++;

        if (dst[0]->length() - dst[0]->position() - 1 < blockSize * 2)
        {
            break;
        }

        src[0]->move(Backward);
        src[1]->move(Backward);
        dst[0]->move(Forward);
    }

    if (end[0] == 0)
    {
        src[1]->move(Backward);
        copy(src[1], dst[1]);
        chunksNumber++;
        return chunksNumber;
    }

    while (true)
    {
        src[0]->move(Backward);
        src[1]->move(Backward);

        end[0] = (src[0]->position() / blockSize) * blockSize;
        end[1] = (src[1]->position() / blockSize) * blockSize;

        mergeTapeChunks(src, end, dst[1], comp);
        chunksNumber++;

        if (src[1]->atBegin())
        {
            break;
        }
        dst[1]->move(Forward);
    }

    if (!src[0]->atBegin())
    {
        chunksNumber++;
        src[0]->move(Backward);
        dst[1]->move(Forward);
        copy(src[0], dst[1]);
    }

    return chunksNumber;
}

} // namespace ts

#endif // SORTER_UTILS_HPP
