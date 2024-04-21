#include <sorter/utils.hpp>

#include <gtest/gtest.h>
#include <cstdlib>
#include <functional>

namespace ts {

namespace {

using IntTape = Tape<int32_t>;

using pos_type = IntTape::pos_type;
using val_type = IntTape::val_type;

using enum MoveDirection;

constexpr size_t TAPE_LEN = 1001;

} // namespace

TEST(SorterUtils, createRandomIntTape)
{
    auto tempTape = createRandomIntTape("SorterUtils_createRandomIntTape_file0", TAPE_LEN);
    ASSERT_EQ(tempTape.length(), TAPE_LEN);
}

TEST(SorterUtils, copy)
{
    auto srcTape = createRandomIntTape("SorterUtils_copy_file0", TAPE_LEN);
    auto dstTape = createTape<val_type>("SorterUtils_mergeTapeChunks_file1", TAPE_LEN);

    std::vector<val_type> testVec(dstTape.length());
    copyToMem(&srcTape, testVec.begin(), testVec.begin() + dstTape.length());
    copy(&srcTape, &dstTape);
    ASSERT_EQ(srcTape.position(), 0);
    ASSERT_EQ(dstTape.position(), dstTape.length() - 1);

    for (auto val : testVec)
    {
        ASSERT_EQ(dstTape.read(), val);
        if (!dstTape.atBegin())
        {
            dstTape.move(Backward);
        }
    }
}

TEST(SorterUtils, mergeTapeChunks)
{
    auto srcTape0 = createTape<val_type>("SorterUtils_mergeTapeChunks_file0", TAPE_LEN);
    auto srcTape1 = createTape<val_type>("SorterUtils_mergeTapeChunks_file1", TAPE_LEN / 2);
    auto dstTape = createTape<val_type>("SorterUtils_mergeTapeChunks_file2", srcTape0.length() + srcTape1.length());

    std::vector<val_type> testVec;

    val_type val = 0;
    for (srcTape0.rewind(Forward); !srcTape0.atBegin(); srcTape0.move(Backward), val += rand() % 3)
    {
        srcTape0.write(val);
        testVec.push_back(val);
    }
    srcTape0.write(val);
    testVec.push_back(val);
    srcTape0.rewind(Forward);

    val = 0;
    for (srcTape1.rewind(Forward); !srcTape1.atBegin(); srcTape1.move(Backward), val += rand() % 3)
    {
        srcTape1.write(val);
        testVec.push_back(val);
    }
    srcTape1.write(val);
    testVec.push_back(val);
    srcTape1.rewind(Forward);

    ITape<val_type>* src[2] = {&srcTape0, &srcTape1};
    ITape<val_type>::pos_type end[2] = {0, 0};
    mergeTapeChunks(src, end, &dstTape, std::less<val_type>());
    ASSERT_TRUE(srcTape0.atBegin());
    ASSERT_TRUE(srcTape1.atBegin());
    ASSERT_TRUE(dstTape.atEnd());

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