#ifndef SORTER_HPP
#define SORTER_HPP

#include "tape.hpp"

#include <vector>

namespace ts {

class Sorter final
{
public:
    using val_type = Tape::val_type;

    Sorter(size_t memsize);

    void sort(const Tape& , Tape* );

private:
    std::vector<val_type> memory_;
};

} // namespace ts

#endif // SORTER_HPP
