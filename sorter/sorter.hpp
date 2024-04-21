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
    using Direction = typename tape_type::MoveDirection;

private:
    using tape_type::MoveDirection::Forward;
    using tape_type::MoveDirection::Backward;

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

        auto tempTapeLength = ((blocksNumber + 1) / 2) * blockSize;
        auto tape0 = tapeFactory(tempTapeLength);
        auto tape1 = tapeFactory(tempTapeLength);
        auto tape2 = tapeFactory(tempTapeLength);
        auto tape3 = tapeFactory(tempTapeLength);

        ITape<T>* tmpSrc[] = {&tape0, &tape1};
        ITape<T>* tmpDst[] = {&tape2, &tape3};

        bool invComp = std::bit_width(blocksNumber - 1) % 2 == 1;
        auto newComp = [&](const auto& a, const auto& b) {
            return invComp ? comp(b, a) : comp(a, b);
        };

        distributeBlocks(src, tmpSrc, newComp);
        invComp = !invComp;

        size_t chunksNumber = 0;
        while (chunksNumber != 2)
        {
            chunksNumber = mergeTapes(tmpSrc, tmpDst, blockSize, newComp);
            if (chunksNumber == 1)
            {
                break;
            }

            std::swap(tmpSrc, tmpDst);
            invComp = !invComp;
            blockSize *= 2;
        }

        using EndPos = typename ITape<T>::pos_type;
        EndPos end[2] = {0, 0};
        mergeTapeChunks(tmpSrc, end, dst, newComp);
    }

    template <typename Compare>
    void distributeBlocks(ITape<T>* src, ITape<T>* dst[2], Compare comp)
    {
        auto blockSize = memory_.size();
        while (true)
        {
            auto remainingSize = src->length() - src->position();
            remainingSize = std::min(remainingSize, dst[0]->length() - dst[0]->position());
            blockSize = std::min(remainingSize, blockSize);

            sortBlock(src, dst[0], blockSize, comp);
            if (src->atEnd())
            {
                break;
            }
            src->move(Forward);

            if (dst[0]->atEnd())
            {
                std::swap(dst[0], dst[1]);
                blockSize = memory_.size();
            }
            else
            {
                dst[0]->move(Forward);
            }
        }
        std::swap(dst[0], dst[1]);
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