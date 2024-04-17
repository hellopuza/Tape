#include <gtest/gtest.h>

#include <tape/utils.hpp>
#include <tape/tape.hpp>

namespace ts {

namespace {

using IntTape = Tape<int32_t>;

using pos_type = IntTape::pos_type;
using val_type = IntTape::val_type;

using enum IntTape::MoveDirection;

const char* FILENAME = "file";
size_t FILESIZE = 1 << 16;

struct UtilsTest : public ::testing::Test
{
    UtilsTest()
    {
        createFile(FILENAME, FILESIZE);
    }
};

} // namespace

TEST_F(UtilsTest, copyToMem)
{
    IntTape tape(FILENAME);
    for (; !tape.atEnd(); tape.move(Forward))
    {
        tape.write(tape.position());
    }
    tape.rewind(Backward);

    std::vector<val_type> mem(tape.length());
    ASSERT_EQ(copyToMem(&tape, mem.begin(), mem.end(), Forward), mem.end());
    tape.rewind(Backward);

    for (; !tape.atEnd(); tape.move(Forward))
    {
        ASSERT_EQ(mem[tape.position()], tape.read());
    }
    ASSERT_EQ(mem[tape.position()], tape.read());
}

TEST_F(UtilsTest, copyFromMem)
{
    IntTape tape(FILENAME);
    std::vector<val_type> mem(tape.length());
    for (size_t i = 0; i < mem.size(); i++)
    {
        mem[i] = i;
    }

    ASSERT_EQ(copyFromMem(mem.begin(), mem.end(), &tape, Forward), mem.end());
    tape.rewind(Backward);

    for (; !tape.atEnd(); tape.move(Forward))
    {
        ASSERT_EQ(tape.read(), mem[tape.position()]);
    }
    ASSERT_EQ(tape.read(), mem[tape.position()]);
}

TEST_F(UtilsTest, createTempTape)
{
    const size_t tempNumber = 11;
    auto tempTape = createTempTape<val_type>(FILESIZE, tempNumber);

    ASSERT_EQ(tempTape.length(), FILESIZE / tempTape.cellSize());
}

} // namespace ts
