#include "itape.hpp"
#include "tape.hpp"

#include <fstream>

namespace ts {

template <typename T, typename It>
It copyToMem(ITape<T>* src, It dFirst, It dLast, typename ITape<T>::MoveDirection dir)
{
    for (; dFirst != dLast; ++dFirst)
    {
        *dFirst = src->read();
        if (src->atEdge(dir))
        {
            ++dFirst;
            break;
        }
        src->move(dir);
    }
    return dFirst;
}

template <typename T, typename It>
It copyFromMem(It sFirst, It sLast, ITape<T>* dst, typename ITape<T>::MoveDirection dir)
{
    for (; sFirst != sLast; ++sFirst)
    {
        dst->write(*sFirst);
        if (dst->atEdge(dir))
        {
            ++sFirst;
            break;
        }
        dst->move(dir);
    }
    return sFirst;
}

static auto createFile(const char* filename, size_t size)
{
    std::ofstream file(filename, std::ios::binary | std::ios::out);
    file.seekp(size - 1);
    file.write("", 1);
    return file;
}

template <typename T>
Tape<T> createTempTape(size_t filesize, size_t number)
{
    static auto tempFilename = std::string("/tmp/tape");
    auto numberedName = tempFilename + std::to_string(number);
    createFile(numberedName.c_str(), filesize);
    return Tape<T>(numberedName.c_str());
}

} // namespace ts