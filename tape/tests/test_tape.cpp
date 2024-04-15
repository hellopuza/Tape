#include <gtest/gtest.h>

#include <tape.hpp>

namespace ts {

using pos_type = Tape::pos_type;
using val_type = Tape::val_type;

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

TEST_F(TapeTest, creation)
{
    Tape tape(FILENAME);
}

TEST_F(TapeTest, creationNoFile)
{
    ASSERT_THROW({ Tape tape(""); }, FileError);
}

TEST_F(TapeTest, position)
{
    Tape tape(FILENAME);

    ASSERT_EQ(tape.position(), 0);
}

TEST_F(TapeTest, length)
{
    Tape tape(FILENAME);

    ASSERT_EQ(tape.length(), FILE_CONTENT.size());
}

TEST_F(TapeTest, moveForward)
{
    Tape tape(FILENAME);

    for (pos_type i = 1; i < FILE_CONTENT.size(); i++)
    {
        tape.moveForward();
        ASSERT_EQ(tape.position(), i);
    }

    ASSERT_THROW(tape.moveForward(), OutOfBorder);
}

TEST_F(TapeTest, moveBackward)
{
    Tape tape(FILENAME);

    for (pos_type i = 1; i < FILE_CONTENT.size(); i++)
    {
        tape.moveForward();
    }

    for (pos_type i = FILE_CONTENT.size() - 1; i > 0; i--)
    {
        ASSERT_EQ(tape.position(), i);
        tape.moveBackward();
    }
    ASSERT_EQ(tape.position(), 0);
    ASSERT_THROW(tape.moveBackward(), OutOfBorder);
}

TEST_F(TapeTest, rewind)
{
    Tape tape(FILENAME);

    tape.moveForward();
    tape.moveForward();
    tape.moveForward();
    tape.rewind();
    ASSERT_EQ(tape.position(), 0);

    tape.rewind();
    ASSERT_EQ(tape.position(), 0);
}

TEST_F(TapeTest, read)
{
    Tape tape(FILENAME);

    for (pos_type i = 1; i < FILE_CONTENT.size(); i++)
    {
        tape.moveForward();
        ASSERT_EQ(tape.read(), FILE_CONTENT[i]);
    }

    for (pos_type i = FILE_CONTENT.size() - 1; i > 0; i--)
    {
        ASSERT_EQ(tape.read(), FILE_CONTENT[i]);
        tape.moveBackward();
    }

    tape.rewind();
    ASSERT_EQ(tape.read(), FILE_CONTENT[0]);
}

TEST_F(TapeTest, write)
{
    Tape tape(FILENAME);

    for (pos_type i = 1; i < FILE_CONTENT.size(); i++)
    {
        tape.moveForward();
        tape.write(i * 10);
        ASSERT_EQ(tape.read(), i * 10);
    }

    for (pos_type i = FILE_CONTENT.size() - 1; i > 0; i--)
    {
        ASSERT_EQ(tape.read(), i * 10);
        tape.moveBackward();
    }

    tape.rewind();
    tape.write(100);
    ASSERT_EQ(tape.read(), 100);
}

TEST(LargeTapeTest, ReadWrite)
{
    const size_t size = 1 << 20;
    const size_t length = size / sizeof(val_type);
    std::vector<val_type> content(length);
    std::ofstream file(FILENAME, std::ios::binary | std::ios::out);
    file.write(reinterpret_cast<const char*>(content.data()), content.size() * sizeof(val_type));

    Tape tape(FILENAME);

    for (pos_type i = 1; i < length; i++)
    {
        tape.moveForward();
        tape.write(i);
    }

    for (pos_type i = length - 1; i > 0; i--)
    {
        ASSERT_EQ(tape.read(), i);
        tape.moveBackward();
    }
}

} // namespace ts
