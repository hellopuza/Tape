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

template <typename T, typename Compare>
static bool isTapeSorted(
    ITape<T>* tape,
    typename ITape<T>::pos_type begin,
    typename ITape<T>::pos_type end,
    Compare comp)
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

} // namespace ts

#endif // SORTER_UTILS_HPP