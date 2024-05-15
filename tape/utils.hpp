#ifndef TAPE_UTILS_HPP
#define TAPE_UTILS_HPP

#include "tape.hpp"

#include <cstdio>
#include <iostream>

namespace ts {

template <typename T, typename It>
void copyToMem(ITape<T>* src, It dFirst, It dLast)
{
    using MoveDirection::Forward;
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
    using MoveDirection::Forward;
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

std::ofstream createFile(const char* filename, size_t size);
void createRandomIntFile(const char* filename, size_t numbersAmount);
Tape<int32_t> createRandomIntTape(const char* filename, size_t numbersAmount);

template <typename T>
Tape<T> createTape(const char* filename, size_t tapeLength)
{
    createFile(filename, tapeLength * sizeof(T));
    return Tape<T>(filename);
}

template <typename T>
struct TempTapeCreator
{
    Tape<T> operator()(size_t tapeLength)
    {
        char name[L_tmpnam];
        if (std::tmpnam(name))
        {
            return createTape<T>(name, tapeLength);
        }
        return createTape<T>("", tapeLength);
    }
};

template <typename T>
typename Tape<T>::Latency loadLatency(std::istream& in)
{
    size_t read, write, move, rewind;
    in >> read >> write >> move >> rewind;

    using Time = typename Tape<T>::time_type;
    return typename Tape<T>::Latency(
        Time(read),
        Time(write),
        Time(move),
        Time(rewind)
    );
}

} // namespace ts

#endif // TAPE_UTILS_HPP