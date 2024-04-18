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
const size_t FILESIZE = 1 << 16;

struct TapeUtils : public ::testing::Test
{
    TapeUtils()
    {
        createFile(FILENAME, FILESIZE);
    }
};

} // namespace

TEST_F(TapeUtils, copyToMem)
{
    IntTape tape(FILENAME);
    for (; !tape.atEnd(); tape.move(Forward))
    {
        tape.write(tape.position());
    }
    tape.rewind(Backward);

    std::vector<val_type> mem(tape.length());
    copyToMem(&tape, mem.begin(), mem.end(), Forward);
    tape.rewind(Backward);

    for (; !tape.atEnd(); tape.move(Forward))
    {
        ASSERT_EQ(mem[tape.position()], tape.read());
    }
    ASSERT_EQ(mem[tape.position()], tape.read());
}

TEST_F(TapeUtils, copyFromMem)
{
    IntTape tape(FILENAME);
    std::vector<val_type> mem(tape.length());
    for (size_t i = 0; i < mem.size(); i++)
    {
        mem[i] = i;
    }

    copyFromMem(&tape, mem.begin(), mem.end(), Forward);
    tape.rewind(Backward);

    for (; !tape.atEnd(); tape.move(Forward))
    {
        ASSERT_EQ(tape.read(), mem[tape.position()]);
    }
    ASSERT_EQ(tape.read(), mem[tape.position()]);
}

TEST_F(TapeUtils, createTempTape)
{
    const size_t tempNumber = 11;
    const size_t tapeLength = 1 << 8;
    auto tempTape = createTempTape<val_type>(tapeLength, tempNumber);

    ASSERT_EQ(tempTape.length(), tapeLength);
}

} // namespace ts
