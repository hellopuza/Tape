#ifndef SORTER_SORTER_HPP
#define SORTER_SORTER_HPP

#include <sorter/utils.hpp>

#include <algorithm>
#include <bit>
#include <functional>

namespace ts {

template <typename T>
class Sorter final
{
public:
    using tape_type = ITape<T>;
    using val_type = typename tape_type::val_type;

private:
    using enum MoveDirection;

public:
    Sorter(size_t memsize)
        : memory_(memsize)
    {}

    template <typename Factory, typename Compare>
    void sort(ITape<T>* src, ITape<T>* dst, Factory tapeFactory, Compare comp)
    {
        auto blockSize = memory_.size();
        auto blocksNumber = (src->length() + blockSize - 1) / blockSize;
        if (blocksNumber == 1)
        {
            sortBlock(src, dst, src->length(), comp);
            return;
        }

        auto tempTapeLength = (blocksNumber / 2 + 1) * blockSize;
        auto tape0 = tapeFactory(tempTapeLength);
        auto tape1 = tapeFactory(tempTapeLength);
        auto tape2 = tapeFactory(tempTapeLength);
        auto tape3 = tapeFactory(tempTapeLength);

        ITape<T>* tmpSrc[] = {&tape0, &tape1};
        ITape<T>* tmpDst[] = {&tape2, &tape3};

        auto sizes0 = createTempTape<size_t>(blocksNumber);
        auto sizes1 = createTempTape<size_t>(blocksNumber);

        Tape<size_t>* sizes[] = {&sizes0, &sizes1};

        bool invComp = std::bit_width(blocksNumber - 1) % 2 == 1;
        auto newComp = [&](const auto& a, const auto& b) {
            return invComp ? comp(b, a) : comp(a, b);
        };

        distributeBlocks(src, tmpSrc, &sizes0, newComp);
        invComp = !invComp;

        while (sizes[0]->position() > 1)
        {
            mergeTapes(tmpSrc, tmpDst, sizes, newComp);
            std::swap(tmpSrc, tmpDst);
            invComp = !invComp;
        }

        using EndPos = typename ITape<T>::pos_type;
        EndPos end[2] = {0, 0};
        mergeTapeChunks(tmpSrc, end, dst, newComp);
    }

    template <typename Compare>
    void distributeBlocks(ITape<T>* src, ITape<T>* dst[2], Tape<size_t>* sizes, Compare comp)
    {
        bool firsIt[2] = {true, true};
        while (true)
        {
            if (!firsIt[0])
            {
                dst[0]->move(Forward);
            }
            firsIt[0] = false;

            auto remainingSize = src->length() - src->position();
            auto blockSize = std::min(remainingSize, memory_.size());

            sizes->write(blockSize);
            sortBlock(src, dst[0], blockSize, comp);
            if (src->atEnd())
            {
                break;
            }

            src->move(Forward);
            sizes->move(Forward);
            std::swap(dst[0], dst[1]);
            std::swap(firsIt[0], firsIt[1]);
        }
    }

    template <typename Compare>
    void sortBlock(ITape<T>* src, ITape<T>* dst, auto size, Compare comp)
    {
        auto begin = memory_.begin();
        auto end = begin + size;

        copyToMem(src, begin, end);
        std::sort(begin, end, comp);
        copyFromMem(dst, begin, end);
    }

private:
    std::vector<val_type> memory_;
};

} // namespace ts

#endif // SORTER_SORTER_HPP