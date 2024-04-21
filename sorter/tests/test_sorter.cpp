#include <sorter/sorter.hpp>

#include <gtest/gtest.h>
#include <functional>
#include <tuple>

namespace ts {

namespace {

using IntTape = Tape<int32_t>;

using pos_type = IntTape::pos_type;
using val_type = IntTape::val_type;

using enum MoveDirection;

using IntSorter = Sorter<val_type>;

} // namespace

class IntSorterTest : public ::testing::TestWithParam<std::tuple<pos_type, pos_type>> {};

TEST_P(IntSorterTest, creation)
{
    const auto MEMSIZE = std::get<0>(GetParam());

    IntSorter sorter(MEMSIZE);
}

TEST_P(IntSorterTest, sortBlock)
{
    const auto TAPE_LEN = std::get<1>(GetParam());
    const auto MEMSIZE = std::min(std::get<0>(GetParam()), TAPE_LEN);

    IntSorter sorter(MEMSIZE);
    auto srcTape = createRandomIntTape("IntSorter_sortBlock_file0", TAPE_LEN);
    auto dstTape = createTape<val_type>("IntSorter_sortBlock_file1", TAPE_LEN);

    std::vector<val_type> testVecSrc(MEMSIZE);
    copyToMem(&srcTape, testVecSrc.begin(), testVecSrc.end());
    srcTape.rewind(Backward);

    sorter.sortBlock(&srcTape, &dstTape, MEMSIZE, std::less<val_type>());
    dstTape.rewind(Backward);

    std::sort(testVecSrc.begin(), testVecSrc.end(), std::less<val_type>());

    std::vector<val_type> testVecDst(MEMSIZE);
    copyToMem(&dstTape, testVecDst.begin(), testVecDst.end());
    ASSERT_EQ(testVecSrc, testVecDst);
}

TEST_P(IntSorterTest, sort)
{
    const auto MEMSIZE = std::get<0>(GetParam());
    const auto TAPE_LEN = std::get<1>(GetParam());

    IntSorter sorter(MEMSIZE);
    auto srcTape = createRandomIntTape("IntSorter_sort_file0", TAPE_LEN);
    auto dstTape = createTape<val_type>("IntSorter_sort_file1", TAPE_LEN);
    sorter.sort(&srcTape, &dstTape, createTempTape<val_type>, std::less<val_type>());

    std::vector<val_type> testVecSrc(TAPE_LEN);
    srcTape.rewind(Backward);
    copyToMem(&srcTape, testVecSrc.begin(), testVecSrc.end());
    std::sort(testVecSrc.begin(), testVecSrc.end(), std::less<val_type>());

    dstTape.rewind(Backward);
    std::vector<val_type> testVecDst(TAPE_LEN);
    copyToMem(&dstTape, testVecDst.begin(), testVecDst.end());
    ASSERT_EQ(testVecSrc, testVecDst);
}

INSTANTIATE_TEST_SUITE_P(
    Sorting,
    IntSorterTest,
    ::testing::Values(
        std::tuple<pos_type, pos_type>(3, 7),
        std::tuple<pos_type, pos_type>(10, 20),
        std::tuple<pos_type, pos_type>(2, 9),
        std::tuple<pos_type, pos_type>(2, 11),
        std::tuple<pos_type, pos_type>(4, 16),
        std::tuple<pos_type, pos_type>(4, 31),
        std::tuple<pos_type, pos_type>(150, 80),
        std::tuple<pos_type, pos_type>(1234, 5000),
        std::tuple<pos_type, pos_type>(3000, 10000)
    )
);

} // namespace ts