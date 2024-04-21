#include "utils.hpp"

namespace ts {

std::ofstream createFile(const char* filename, size_t size)
{
    std::ofstream file(filename, std::ios::binary | std::ios::out);
    file.seekp(size - 1);
    file.write("", 1);
    file.seekp(0);
    return file;
}

} // namespace ts