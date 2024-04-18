#ifndef TAPE_UTILS_HPP
#define TAPE_UTILS_HPP

#include "itape.hpp"
#include "tape.hpp"

#include <fstream>

namespace ts {

template <typename T, typename It, typename Dir = typename ITape<T>::MoveDirection>
void copyToMem(ITape<T>* src, It dFirst, It dLast, Dir dir)
{
    while (dFirst != dLast)
    {
        *dFirst = src->read();
        ++dFirst;

        if (!((dFirst == dLast) && src->atEdge(dir)))
        {
            src->move(dir);
        }
    }
}

template <typename T, typename It, typename Dir = typename ITape<T>::MoveDirection>
void copyFromMem(ITape<T>* dst, It sFirst, It sLast, Dir dir)
{
    while (sFirst != sLast)
    {
        dst->write(*sFirst);
        ++sFirst;

        if (!((sFirst == sLast) && dst->atEdge(dir)))
        {
            dst->move(dir);
        }
    }
}

static auto createFile(const char* filename, size_t size)
{
    std::ofstream file(filename, std::ios::binary | std::ios::out);
    file.seekp(size - 1);
    file.write("", 1);
    return file;
}

template <typename T>
Tape<T> createTempTape(size_t tapeLength, size_t number)
{
    static auto tempFilename = std::string("/tmp/tape");
    auto numberedName = tempFilename + std::to_string(number);
    createFile(numberedName.c_str(), tapeLength * sizeof(T));
    return Tape<T>(numberedName.c_str());
}

} // namespace ts

#endif // TAPE_UTILS_HPP