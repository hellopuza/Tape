#include "utils.hpp"

#include <random>

namespace ts {

std::ofstream createFile(const char* filename, size_t size)
{
    std::ofstream file(filename, std::ios::binary | std::ios::out);
    file.seekp(size - 1);
    file.write("", 1);
    file.seekp(0);
    return file;
}

void createRandomIntFile(const char* filename, size_t numbersAmount)
{
    auto file = createFile(filename, numbersAmount * sizeof(int32_t));

    std::random_device randD;
    std::mt19937 randMT(randD());
    std::uniform_int_distribution<int32_t> range;

    for (size_t i = 0; i < numbersAmount; i++)
    {
        const int32_t val = range(randMT);
        file.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }
}

Tape<int32_t> createRandomIntTape(const char* filename, size_t numbersAmount)
{
    createRandomIntFile(filename, numbersAmount);
    return Tape<int32_t>(filename);
}

} // namespace ts