#include <gtest/gtest.h>
#include <functional>

#include <sorter/utils.hpp>
#include <sorter/sorter.hpp>
#include <tape/tape.hpp>

namespace ts {

namespace {

using IntTape = Tape<int32_t>;

using pos_type = IntTape::pos_type;
using val_type = IntTape::val_type;

using enum IntTape::MoveDirection;

using IntSorter = Sorter<val_type>;

constexpr size_t MEMSIZE = 1 << 10;
constexpr size_t TAPE_LEN = 1 << 16;

} // namespace

TEST(IntSorter, creation) {
    IntSorter sorter(MEMSIZE);
}

TEST(IntSorter, sortBlock) {
    IntSorter sorter(MEMSIZE);
    auto srcTape = createRandomIntTape("src_tape", TAPE_LEN);
    auto dstTape = createTempTape<val_type>(TAPE_LEN, 0);

    ASSERT_FALSE(sorter.sortBlock(&srcTape, &dstTape, Forward, Forward, std::less<val_type>()));
    ASSERT_TRUE(isTapeSorted(&dstTape, 0, MEMSIZE - 1, std::less<val_type>()));

    dstTape.move(Backward);
    ASSERT_TRUE(sorter.sortBlock(&srcTape, &dstTape, Forward, Backward, std::greater<val_type>()));
    ASSERT_TRUE(isTapeSorted(&dstTape, MEMSIZE - 1, 0, std::greater<val_type>()));
}

} // namespace ts
