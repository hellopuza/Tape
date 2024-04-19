#ifndef SORTER_SORTER_HPP
#define SORTER_SORTER_HPP

#include <sorter/utils.hpp>

#include <algorithm>

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
        auto tempTapeLength = ((blocksNumber + 1) / 2) * blockSize;

        auto tape1 = tapeFactory(tempTapeLength);
        auto tape2 = tapeFactory(tempTapeLength);
        auto tape3 = tapeFactory(tempTapeLength);
        auto tape4 = tapeFactory(tempTapeLength);

        distributeBlocks(src, &tape1, &tape2, comp);
    }

    template <typename Compare>
    void distributeBlocks(ITape<T>* src, ITape<T>* dst1, ITape<T>* dst2, Compare comp)
    {
        bool lastIteration = false;
        auto blockSize = std::min(memory_.size(), src->length());
        do
        {
            auto remainingSize = src->length() - src->position();
            blockSize = std::min(remainingSize, blockSize);
            lastIteration = (remainingSize == blockSize);

            sortBlock(src, dst1, blockSize, comp);
            if (dst1->atEnd())
            {
                std::swap(dst1, dst2);
            }
        } while (!lastIteration);
    }

    template <typename Compare>
    void sortBlock(ITape<T>* src, ITape<T>* dst, auto size, Compare comp)
    {
        auto begin = memory_.begin();
        auto end = begin + size;

        copyToMem(src, begin, end, Forward);
        std::sort(begin, end, comp);
        copyFromMem(dst, begin, end, Forward);
    }

private:
    std::vector<val_type> memory_;
};

} // namespace ts

#endif // SORTER_SORTER_HPP
