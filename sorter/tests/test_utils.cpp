#include <gtest/gtest.h>
#include <functional>

#include <sorter/utils.hpp>
#include <tape/tape.hpp>

namespace ts {

namespace {

using IntTape = Tape<int32_t>;

using pos_type = IntTape::pos_type;
using val_type = IntTape::val_type;

using enum IntTape::MoveDirection;

constexpr size_t TAPE_LEN = 1 << 8;

} // namespace

TEST(SorterUtils, isTapeSortedAscending) {
    auto tape = createTempTape<val_type>(TAPE_LEN, 0);

    val_type val = 0;
    for (; !tape.atEnd(); tape.move(Forward), val++)
    {
        tape.write(val);
    }
    tape.write(val);

    ASSERT_TRUE(isTapeSorted(&tape, 0, tape.length() - 1, std::less<val_type>()));

    ASSERT_TRUE(isTapeSorted(&tape, tape.length() - 1, 0, std::greater<val_type>()));

    ASSERT_FALSE(isTapeSorted(&tape, tape.length() / 2, 0, std::less<val_type>()));

    tape.rewind(Forward);
    tape.write(0);
    ASSERT_FALSE(isTapeSorted(&tape, 0, tape.length() - 1, std::less<val_type>()));
}

TEST(SorterUtils, createRandomIntTape) {
    auto tempTape = createRandomIntTape("file", TAPE_LEN);
    ASSERT_EQ(tempTape.length(), TAPE_LEN);
}

} // namespace ts
