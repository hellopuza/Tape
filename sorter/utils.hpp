#include <tape/utils.hpp>

#include <random>

namespace ts {

static auto createRandomIntFile(const char* filename, size_t numbersAmount)
{
    auto file = createFile(filename, numbersAmount * sizeof(int32_t));

    std::random_device randD;
    std::mt19937 randMT(randD());
    std::uniform_int_distribution<int32_t> range(0, 1 << 31);

    file.seekp(0);
    for (size_t i = 0; i < numbersAmount; i++)
    {
        int32_t val = range(randMT);
        file.write(&val, sizeof(val));
    }
    return file;
}

} // namespace ts