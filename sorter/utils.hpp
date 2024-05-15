#ifndef SORTER_UTILS_HPP
#define SORTER_UTILS_HPP

#include <tape/itape.hpp>

#include <algorithm>

namespace ts {

template <typename T, typename EndPos = typename ITape<T>::pos_type>
void copy(ITape<T>* src, ITape<T>* dst, EndPos end = 0)
{
    using enum MoveDirection;

    while (true)
    {
        dst->write(src->read());
        if (src->position() == end)
        {
            break;
        }
        src->move(Backward);
        dst->move(Forward);
    }
}

template <typename T, typename Compare, typename EndPos = typename ITape<T>::pos_type>
void mergeTapeChunks(ITape<T>* src[2], EndPos end[2], ITape<T>* dst, Compare comp)
{
    using val_type = typename ITape<T>::val_type;
    using enum MoveDirection;

    val_type val[2] = {src[0]->read(), src[1]->read()};
    int n;
    while (true)
    {
        n = static_cast<int>(!comp(val[0], val[1]));
        dst->write(val[n]);
        dst->move(Forward);

        if (src[n]->position() == end[n])
        {
            n = 1 - n;
            break;
        }

        src[n]->move(Backward);
        val[n] = src[n]->read();
    }

    copy(src[n], dst, end[n]);
}

template <typename T, typename Compare>
void mergeTapes(ITape<T>* src[2], ITape<T>* dst[2], ITape<size_t>* sizes[2], Compare comp)
{
    using EndPos = typename ITape<T>::pos_type;
    using enum MoveDirection;

    bool firsIt[2] = {true, true};
    auto updateDst = [&]() {
        if (!firsIt[0])
        {
            dst[0]->move(Forward);
        }
        firsIt[0] = false;
        std::swap(firsIt[0], firsIt[1]);
    };

    while (true)
    {
        updateDst();

        auto size0 = sizes[0]->read();
        sizes[0]->move(Backward);
        auto size1 = sizes[0]->read();
        sizes[1]->write(size0 + size1);

        EndPos end[] = {
            src[0]->position() - size0 + 1,
            src[1]->position() - size1 + 1
        };
        mergeTapeChunks(src, end, dst[0], comp);

        if (src[0]->atBegin() || src[1]->atBegin())
        {
            break;
        }

        src[0]->move(Backward);
        src[1]->move(Backward);
        sizes[0]->move(Backward);
        sizes[1]->move(Forward);
        std::swap(dst[0], dst[1]);
    }

    for (int i = 0; i < 2; i++)
    {
        if (!src[i]->atBegin())
        {
            std::swap(dst[0], dst[1]);
            updateDst();

            src[i]->move(Backward);
            sizes[0]->move(Backward);
            sizes[1]->move(Forward);
            sizes[1]->write(sizes[0]->read());
            copy(src[i], dst[0]);
        }
    }
    std::swap(sizes[0], sizes[1]);
}

} // namespace ts

#endif // SORTER_UTILS_HPP