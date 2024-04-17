#ifndef SORTER_HPP
#define SORTER_HPP

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

    bool sortBlock(tape_type* src, tape_type* dst, tape_type::MoveDirection dir)
    {
        auto memEnd = copyToMem(src, memory_.begin(), memory_.end(), dir);
        std::sort(memory_.begin(), memEnd);
        memEnd = copyFromMem(memory_.begin(), memEnd, dst, dir);

        bool endOfDst = memory_.begin() != memEnd;
        return endOfDst;
    }

private:
    std::vector<val_type> memory_;
};

} // namespace ts

#endif // SORTER_HPP
