#include <gtest/gtest.h>
#include <cstdlib>
#include <functional>

#include <iostream>

#include <sorter/utils.hpp>
#include <tape/tape.hpp>

namespace ts {

namespace {

using IntTape = Tape<int32_t>;

using pos_type = IntTape::pos_type;
using val_type = IntTape::val_type;

using enum IntTape::MoveDirection;

using IntTapeChunk = TapeChunk<val_type>;
using IntTapeDirected = TapeDirected<val_type>;

constexpr size_t TAPE_LEN = 1 << 4;

} // namespace

TEST(SorterUtils, createRandomIntTape) {
    auto tempTape = createRandomIntTape("SorterUtils_file0", TAPE_LEN);
    ASSERT_EQ(tempTape.length(), TAPE_LEN);
}

TEST(SorterUtils, mergeTapeChunks) {
    auto srcTape0 = createTempTape<val_type>(TAPE_LEN, 0);
    auto srcTape1 = createTempTape<val_type>(TAPE_LEN / 2, 1);
    auto dstTape = createTempTape<val_type>(srcTape0.length() + srcTape1.length(), 2);

    std::vector<val_type> testVec;

    val_type val = 0;
    for (; !srcTape0.atEnd(); srcTape0.move(Forward), val += rand() % 3)
    {
        srcTape0.write(val);
        testVec.push_back(val);
    }
    srcTape0.write(val);
    srcTape0.rewind(Backward);
    testVec.push_back(val);

    val = 0;
    for (; !srcTape1.atEnd(); srcTape1.move(Forward), val += rand() % 3)
    {
        srcTape1.write(val);
        testVec.push_back(val);
    }
    srcTape1.write(val);
    srcTape1.rewind(Backward);
    testVec.push_back(val);

    IntTapeChunk chunk[2] = {
        IntTapeChunk{&srcTape0, Forward, srcTape0.length() - 1},
        IntTapeChunk{&srcTape1, Forward, srcTape1.length() - 1}
    };
    mergeTapeChunks(chunk, IntTapeDirected{&dstTape, Forward}, std::less<val_type>());

    std::sort(testVec.begin(), testVec.end(), std::less<val_type>());
    dstTape.rewind(Backward);

    auto it = testVec.begin();
    for (; !dstTape.atEnd(); dstTape.move(Forward))
    {
        ASSERT_EQ(dstTape.read(), *it);
        ++it;
    }
    ASSERT_EQ(dstTape.read(), *it);
}

} // namespace ts
