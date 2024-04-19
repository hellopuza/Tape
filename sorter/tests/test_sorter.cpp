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
    auto srcTape = createRandomIntTape("IntSorter_file0", TAPE_LEN);
    auto dstTape = createTempTape<val_type>(TAPE_LEN, 0);

    std::vector<val_type> testVecSrc(MEMSIZE);
    copyToMem(&srcTape, testVecSrc.begin(), testVecSrc.end(), Forward);
    srcTape.rewind(Backward);

    sorter.sortBlock(&srcTape, &dstTape, MEMSIZE, std::less<val_type>());
    dstTape.rewind(Backward);

    std::sort(testVecSrc.begin(), testVecSrc.end(), std::less<val_type>());

    std::vector<val_type> testVecDst(MEMSIZE);
    copyToMem(&dstTape, testVecDst.begin(), testVecDst.end(), Forward);
    ASSERT_EQ(testVecSrc, testVecDst);
}

TEST(IntSorter, distributeBlocks) {
    IntSorter sorter(MEMSIZE);
    auto srcTape = createRandomIntTape("IntSorter_file1", TAPE_LEN);
    auto dstTape0 = createTempTape<val_type>(TAPE_LEN / 2, 0);
    auto dstTape1 = createTempTape<val_type>(TAPE_LEN / 2, 1);

    sorter.distributeBlocks(&srcTape, &dstTape0, &dstTape1, std::less<val_type>());
    srcTape.rewind(Backward);

    std::vector<val_type> testVecSrc(MEMSIZE);
    std::vector<val_type> testVecDst(MEMSIZE);

    dstTape0.rewind(Backward);
    while (!dstTape0.atEnd())
    {
        copyToMem(&dstTape0, testVecDst.begin(), testVecDst.end(), Forward);
        copyToMem(&srcTape, testVecSrc.begin(), testVecSrc.end(), Forward);
        std::sort(testVecSrc.begin(), testVecSrc.end(), std::less<val_type>());
        ASSERT_EQ(testVecSrc, testVecDst);
    }

    dstTape1.rewind(Backward);
    while (!dstTape1.atEnd())
    {
        copyToMem(&dstTape1, testVecDst.begin(), testVecDst.end(), Forward);
        copyToMem(&srcTape, testVecSrc.begin(), testVecSrc.end(), Forward);
        std::sort(testVecSrc.begin(), testVecSrc.end(), std::less<val_type>());
        ASSERT_EQ(testVecSrc, testVecDst);
    }
}

} // namespace ts
