#include <gtest/gtest.h>
#include <cstdlib>
#include <functional>

#include <sorter/utils.hpp>
#include <tape/tape.hpp>

namespace ts {

namespace {

using IntTape = Tape<int32_t>;

using pos_type = IntTape::pos_type;
using val_type = IntTape::val_type;

using enum IntTape::MoveDirection;

using IntTapeChunk = TapeChunk<val_type>;

constexpr size_t TAPE_LEN = 1 << 8;

} // namespace

TEST(SorterUtils, isTapeSorted) {
    auto tape = createTempTape<val_type>(TAPE_LEN, 0);

    val_type val = 0;
    for (; !tape.atEnd(); tape.move(Forward), val++)
    {
        tape.write(val);
    }
    tape.write(val);

    ASSERT_TRUE(isTapeSorted(&tape, 0lu, tape.length() - 1, std::less<val_type>()));

    ASSERT_TRUE(isTapeSorted(&tape, tape.length() - 1, 0lu, std::greater<val_type>()));

    ASSERT_FALSE(isTapeSorted(&tape, tape.length() / 2, 0lu, std::less<val_type>()));

    tape.rewind(Forward);
    tape.write(0);
    ASSERT_FALSE(isTapeSorted(&tape, 0lu, tape.length() - 1, std::less<val_type>()));
}

TEST(SorterUtils, createRandomIntTape) {
    auto tempTape = createRandomIntTape("file", TAPE_LEN);
    ASSERT_EQ(tempTape.length(), TAPE_LEN);
}

TEST(SorterUtils, mergeTapes) {
    auto srcTape0 = createTempTape<val_type>(TAPE_LEN, 0);
    auto srcTape1 = createTempTape<val_type>(TAPE_LEN / 2, 1);
    auto dstTape = createTempTape<val_type>(srcTape0.length() + srcTape1.length(), 2);

    val_type val = 0;
    for (; !srcTape0.atEnd(); srcTape0.move(Forward), val += rand() % 3)
    {
        srcTape0.write(val);
    }
    srcTape0.write(val);
    srcTape0.rewind(Backward);

    val = 0;
    for (; !srcTape1.atEnd(); srcTape1.move(Forward), val += rand() % 3)
    {
        srcTape1.write(val);
    }
    srcTape1.write(val);
    srcTape1.rewind(Backward);

    IntTapeChunk chunk[2] = {
        IntTapeChunk{&srcTape0, Forward, srcTape0.length() - 1},
        IntTapeChunk{&srcTape1, Forward, srcTape1.length() - 1}
    };
    mergeTapes(chunk, &dstTape, Forward, std::less<val_type>());

    ASSERT_TRUE(isTapeSorted(&dstTape, 0lu, dstTape.length() - 1, std::less<val_type>()));
}

} // namespace ts
