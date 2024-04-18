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

template <typename T, typename Compare, typename Pos = typename ITape<T>::pos_type>
bool isTapeSorted(ITape<T>* tape, Pos begin, Pos end, Compare comp)
{
    using ITape<T>::MoveDirection::Forward;
    using ITape<T>::MoveDirection::Backward;

    auto dir = tape->position() < begin ? Forward : Backward;
    for (; tape->position() != begin; tape->move(dir));

    dir = end > begin ? Forward : Backward;
    auto prev_val = tape->read();
    for (; tape->position() != end; tape->move(dir))
    {
        auto val = tape->read();
        if (comp(val, prev_val))
        {
            return false;
        }
        prev_val = val;
    }
    return !comp(tape->read(), prev_val);
}

template <typename T>
struct TapeChunk
{
    using Dir = typename ITape<T>::MoveDirection;
    using EndPos = typename ITape<T>::pos_type;

    ITape<T>* tape;
    Dir dir;
    EndPos end;
};

template <typename T, typename Compare, typename Dir = typename TapeChunk<T>::Dir>
void mergeTapes(TapeChunk<T> src[2], ITape<T>* dst, Dir dstDir, Compare comp)
{
    using val_type = typename ITape<T>::val_type;

    while (true)
    {
        val_type val[2] = {src[0].tape->read(), src[1].tape->read()};
        auto res = static_cast<int>(comp(val[0], val[1]));

        dst->write(val[res]);

        if (  (src[0].tape->position() == src[0].end)
           && (src[1].tape->position() == src[1].end))
        {
            break;
        }
        dst->move(dstDir);

        if (src[res].tape->position() != src[res].end)
        {
            src[res].tape->move(src[res].dir);
        }
        else
        {
            src[res] = src[1 - res];
        }
    }
}

} // namespace ts

#endif // SORTER_UTILS_HPP