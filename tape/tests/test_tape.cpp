#include <gtest/gtest.h>

#include <tape/tape.hpp>
#include <tape/utils.hpp>

namespace ts {

namespace {

using IntTape = Tape<int32_t>;

using pos_type = IntTape::pos_type;
using val_type = IntTape::val_type;

using enum IntTape::MoveDirection;

const char* FILENAME = "file";
const std::vector<val_type> FILE_CONTENT = {1, 2, 3, 4, 5, 6, 7, 8};

struct TapeTest : public ::testing::Test
{
    TapeTest()
    {
        std::ofstream file(FILENAME, std::ios::binary | std::ios::out);
        file.write(reinterpret_cast<const char*>(FILE_CONTENT.data()), FILE_CONTENT.size() * sizeof(val_type));
    }
};

} // namespace

TEST_F(TapeTest, creation)
{
    IntTape tape(FILENAME);
}

TEST_F(TapeTest, creationNoFile)
{
    ASSERT_THROW({ IntTape tape(""); }, FileError);
}

TEST_F(TapeTest, position)
{
    IntTape tape(FILENAME);

    ASSERT_EQ(tape.position(), 0);
}

TEST_F(TapeTest, length)
{
    IntTape tape(FILENAME);

    ASSERT_EQ(tape.length(), FILE_CONTENT.size());
}

TEST_F(TapeTest, atBegin)
{
    IntTape tape(FILENAME);

    ASSERT_EQ(tape.atBegin(), true);
    tape.move(Forward);
    ASSERT_EQ(tape.atBegin(), false);
}

TEST_F(TapeTest, atEnd)
{
    IntTape tape(FILENAME);

    ASSERT_EQ(tape.atEnd(), false);
    tape.rewind(Forward);
    ASSERT_EQ(tape.atEnd(), true);
}

TEST_F(TapeTest, atEdge)
{
    IntTape tape(FILENAME);

    ASSERT_EQ(tape.atEdge(Backward), true);
    ASSERT_EQ(tape.atEdge(Forward), false);

    tape.move(Forward);
    ASSERT_EQ(tape.atEdge(Backward), false);
    ASSERT_EQ(tape.atEdge(Forward), false);

    tape.rewind(Forward);
    ASSERT_EQ(tape.atEdge(Backward), false);
    ASSERT_EQ(tape.atEdge(Forward), true);
}

TEST_F(TapeTest, moveForward)
{
    IntTape tape(FILENAME);

    for (pos_type i = 1; i < FILE_CONTENT.size(); i++)
    {
        tape.move(Forward);
        ASSERT_EQ(tape.position(), i);
    }
    ASSERT_EQ(tape.atEnd(), true);

    ASSERT_THROW(tape.move(Forward), OutOfBorder);
}

TEST_F(TapeTest, moveBackward)
{
    IntTape tape(FILENAME);

    tape.rewind(Forward);

    for (pos_type i = FILE_CONTENT.size() - 1; i > 0; i--)
    {
        ASSERT_EQ(tape.position(), i);
        tape.move(Backward);
    }
    ASSERT_EQ(tape.position(), 0);
    ASSERT_THROW(tape.move(Backward), OutOfBorder);
}

TEST_F(TapeTest, rewind)
{
    IntTape tape(FILENAME);

    tape.move(Forward);
    tape.move(Forward);
    tape.move(Forward);
    tape.rewind(Backward);
    ASSERT_EQ(tape.position(), 0);

    tape.rewind(Backward);
    ASSERT_EQ(tape.position(), 0);

    tape.rewind(Forward);
    ASSERT_EQ(tape.position(), tape.length() - 1);
}

TEST_F(TapeTest, read)
{
    IntTape tape(FILENAME);

    for (pos_type i = 1; i < FILE_CONTENT.size(); i++)
    {
        tape.move(Forward);
        ASSERT_EQ(tape.read(), FILE_CONTENT[i]);
    }

    for (pos_type i = FILE_CONTENT.size() - 1; i > 0; i--)
    {
        ASSERT_EQ(tape.read(), FILE_CONTENT[i]);
        tape.move(Backward);
    }

    tape.rewind(Backward);
    ASSERT_EQ(tape.read(), FILE_CONTENT[0]);
}

TEST_F(TapeTest, write)
{
    IntTape tape(FILENAME);

    for (pos_type i = 1; i < FILE_CONTENT.size(); i++)
    {
        tape.move(Forward);
        tape.write(i * 10);
        ASSERT_EQ(tape.read(), i * 10);
    }

    for (pos_type i = FILE_CONTENT.size() - 1; i > 0; i--)
    {
        ASSERT_EQ(tape.read(), i * 10);
        tape.move(Backward);
    }

    tape.rewind(Backward);
    tape.write(100);
    ASSERT_EQ(tape.read(), 100);
}

TEST(LargeTapeTest, ReadWrite)
{
    const size_t size = 1 << 16;
    const size_t length = size / sizeof(val_type);
    createFile(FILENAME, size);

    IntTape tape(FILENAME);

    for (pos_type i = 1; i < length; i++)
    {
        tape.move(Forward);
        tape.write(i);
    }

    for (pos_type i = length - 1; i > 0; i--)
    {
        ASSERT_EQ(tape.read(), i);
        tape.move(Backward);
    }
}

} // namespace ts
