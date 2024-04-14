#include <gtest/gtest.h>

#include <tape.hpp>

namespace ts {

const char* FILENAME = "file";

struct TapeTest : public ::testing::Test
{
    TapeTest()
        : file(FILENAME, std::ios::binary | std::ios::out)
    {
        int32_t arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
        file.write(reinterpret_cast<const char*>(arr), sizeof(arr));
        file.close();
    }

    std::ofstream file;
};

TEST_F(TapeTest, creation)
{
    Tape tape(FILENAME);
}

TEST_F(TapeTest, creationNoFile)
{
    ASSERT_THROW({ Tape tape(""); }, FileError);
}

TEST_F(TapeTest, getPosition)
{
    Tape tape(FILENAME);

    ASSERT_EQ(tape.getPosition(), 0);
}

TEST_F(TapeTest, moveForward)
{
    Tape tape(FILENAME);

    tape.moveForward();
    ASSERT_EQ(tape.getPosition(), tape.cellSize());

    tape.moveForward();
    ASSERT_EQ(tape.getPosition(), tape.cellSize() * 2);

    tape.moveForward();
    ASSERT_EQ(tape.getPosition(), tape.cellSize() * 3);
}

TEST_F(TapeTest, moveBackward)
{
    Tape tape(FILENAME);

    tape.moveForward();
    tape.moveForward();
    tape.moveBackward();
    ASSERT_EQ(tape.getPosition(), tape.cellSize());

    tape.moveBackward();
    ASSERT_EQ(tape.getPosition(), 0);

    tape.moveForward();
    ASSERT_EQ(tape.getPosition(), tape.cellSize());

    tape.moveBackward();
    ASSERT_EQ(tape.getPosition(), 0);
}

TEST_F(TapeTest, rewind)
{
    Tape tape(FILENAME);

    tape.moveForward();
    tape.moveForward();
    tape.moveForward();
    tape.rewind();
    ASSERT_EQ(tape.getPosition(), 0);

    tape.rewind();
    ASSERT_EQ(tape.getPosition(), 0);
}

TEST_F(TapeTest, read)
{
    Tape tape(FILENAME);

    ASSERT_EQ(tape.read(), 1);

    tape.moveForward();
    ASSERT_EQ(tape.read(), 2);

    tape.moveForward();
    ASSERT_EQ(tape.read(), 3);

    tape.moveBackward();
    ASSERT_EQ(tape.read(), 2);

    tape.moveForward();
    tape.moveForward();
    tape.rewind();
    ASSERT_EQ(tape.read(), 1);
}

TEST_F(TapeTest, write)
{
    Tape tape(FILENAME);

    tape.write(10);
    ASSERT_EQ(tape.read(), 10);

    tape.write(20);
    ASSERT_EQ(tape.read(), 20);

    tape.moveForward();
    tape.write(30);

    tape.moveForward();
    tape.write(40);
    ASSERT_EQ(tape.read(), 40);

    tape.moveBackward();
    ASSERT_EQ(tape.read(), 30);

    tape.rewind();
    tape.write(100);
    ASSERT_EQ(tape.read(), 100);
}

} // namespace ts
