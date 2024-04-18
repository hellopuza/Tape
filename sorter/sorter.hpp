#ifndef SORTER_SORTER_HPP
#define SORTER_SORTER_HPP

#include <tape/itape.hpp>
#include <sorter/utils.hpp>

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

    void sort(ITape<T>*, ITape<T>*)
    {}

    template <typename Compare, typename Dir = typename ITape<T>::MoveDirection>
    void sortBlock(
        ITape<T>* src, ITape<T>* dst,
        Dir srcDir, Dir dstDir,
        Compare comp)
    {
        copyToMem(src, memory_.begin(), memory_.end(), srcDir);
        std::sort(memory_.begin(), memory_.end(), comp);
        copyFromMem(dst, memory_.begin(), memory_.end(), dstDir);
    }

private:
    std::vector<val_type> memory_;
};

} // namespace ts

#endif // SORTER_SORTER_HPP