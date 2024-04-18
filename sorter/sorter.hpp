#ifndef SORTER_SORTER_HPP
#define SORTER_SORTER_HPP

#include <tape/itape.hpp>
#include <tape/utils.hpp>

#include <algorithm>

namespace ts {

template <typename T>
class Sorter final
{
public:
    using tape_type = ITape<T>;
    using val_type = typename tape_type::val_type;

private:
    using tape_type::MoveDirection::Forward;
    using tape_type::MoveDirection::Backward;

public:
    Sorter(size_t memsize)
        : memory_(memsize)
    {}

    void sort(tape_type*, tape_type*)
    {}

    template <typename Compare>
    bool sortBlock(
        tape_type* src, tape_type* dst,
        tape_type::MoveDirection srcDir,
        tape_type::MoveDirection dstDir,
        Compare comp)
    {
        auto memEnd = copyToMem(src, memory_.begin(), memory_.end(), srcDir);
        std::sort(memory_.begin(), memEnd, comp);
        memEnd = copyFromMem(memory_.begin(), memEnd, dst, dstDir);

        bool endOfDst = memory_.end() != memEnd;
        return endOfDst;
    }

private:
    std::vector<val_type> memory_;
};

} // namespace ts

#endif // SORTER_SORTER_HPP