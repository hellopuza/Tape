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
    std::uniform_int_distribution<int32_t> range;

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

template <typename T>
struct TapeDirected
{
    using Direction = typename ITape<T>::MoveDirection;

    ITape<T>* tape;
    Direction dir;
};

template <typename T>
struct TapeChunk
{
    using Direction = typename ITape<T>::MoveDirection;
    using EndPos = typename ITape<T>::pos_type;

    ITape<T>* tape;
    Direction dir;
    EndPos end;
};

template <typename T, typename Compare>
void mergeTapeChunks(TapeChunk<T> src[2], TapeDirected<T> dst, Compare comp)
{
    using val_type = typename ITape<T>::val_type;

    val_type val[2] = {src[0].tape->read(), src[1].tape->read()};
    int n;
    while (true)
    {
        n = static_cast<int>(!comp(val[0], val[1]));
        dst.tape->write(val[n]);
        dst.tape->move(dst.dir);

        if (src[n].tape->position() == src[n].end)
        {
            n = 1 - n;
            break;
        }

        src[n].tape->move(src[n].dir);
        val[n] = src[n].tape->read();
    }

    while (true)
    {
        dst.tape->write(src[n].tape->read());
        if (!dst.tape->atEdge(dst.dir))
        {
            dst.tape->move(dst.dir);
        }
        if (src[n].tape->position() == src[n].end)
        {
            break;
        }
        src[n].tape->move(src[n].dir);
    }
}

template <typename T, typename Compare>
void mergeTapes(TapeDirected<T>[2] src, TapeDirected<T>[2] dst, auto blockSize, Compare comp)
{
}

} // namespace ts

#endif // SORTER_UTILS_HPP
