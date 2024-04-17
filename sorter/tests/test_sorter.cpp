#include <gtest/gtest.h>

#include <sorter/sorter.hpp>
#include <tape/tape.hpp>

namespace ts {

namespace {

using IntTape = Tape<int32_t>;

using pos_type = IntTape::pos_type;
using val_type = IntTape::val_type;

using IntSorter = Sorter<val_type>;

constexpr size_t MEMSIZE = 1 << 10;
constexpr size_t FILESIZE = 1 << 16;

} // namespace

TEST(IntSorter, creation) {
    IntSorter sorter(MEMSIZE);
}

TEST(IntSorter, sortBlock) {
    IntSorter sorter(MEMSIZE);
    auto tempTape0 = createTempTape<val_type>(FILESIZE, 0);
    auto tempTape1 = createTempTape<val_type>(FILESIZE, 1);
}

} // namespace ts
