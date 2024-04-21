#ifndef TAPE_UTILS_HPP
#define TAPE_UTILS_HPP

#include "itape.hpp"
#include "tape.hpp"

#include <cstdio>

namespace ts {

template <typename T, typename It>
void copyToMem(ITape<T>* src, It dFirst, It dLast)
{
    using ITape<T>::MoveDirection::Forward;
    while (dFirst != dLast)
    {
        *dFirst = src->read();
        ++dFirst;

        if (dFirst != dLast)
        {
            src->move(Forward);
        }
    }
}

template <typename T, typename It>
void copyFromMem(ITape<T>* dst, It sFirst, It sLast)
{
    using ITape<T>::MoveDirection::Forward;
    while (sFirst != sLast)
    {
        dst->write(*sFirst);
        ++sFirst;

        if (sFirst != sLast)
        {
            dst->move(Forward);
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
Tape<T> createTape(const char* filename, size_t tapeLength)
{
    createFile(filename, tapeLength * sizeof(T));
    return Tape<T>(filename);
}

template <typename T>
Tape<T> createTempTape(size_t tapeLength)
{
    char name[L_tmpnam];
    if (std::tmpnam(name))
    {
        return createTape<T>(name, tapeLength);
    }
    return createTape<T>("", tapeLength);
}

} // namespace ts

#endif // TAPE_UTILS_HPP
