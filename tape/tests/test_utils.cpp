#include <tape/utils.hpp>

#include <gtest/gtest.h>

namespace ts {

namespace {

using IntTape = Tape<int32_t>;

using pos_type = IntTape::pos_type;
using val_type = IntTape::val_type;

using enum IntTape::MoveDirection;

constexpr size_t TAPE_LEN = 1 << 10;
constexpr size_t MEMSIZE = 1 << 8;

} // namespace

TEST(TapeUtils, copyToMem)
{
    auto tape = createTape<val_type>("TapeUtils_copyToMem_file0", TAPE_LEN);
    for (; !tape.atEnd(); tape.move(Forward))
    {
        tape.write(tape.position());
    }
    tape.write(tape.position());
    tape.rewind(Backward);

    std::vector<val_type> mem(MEMSIZE);
    copyToMem(&tape, mem.begin(), mem.end());
    ASSERT_EQ(tape.position(), MEMSIZE - 1);
    tape.rewind(Backward);

    for (; tape.position() != MEMSIZE - 1; tape.move(Forward))
    {
        ASSERT_EQ(mem[tape.position()], tape.read());
    }
    ASSERT_EQ(mem[tape.position()], tape.read());
}

TEST(TapeUtils, copyFromMem)
{
    auto tape = createTape<val_type>("TapeUtils_copyFromMem_file0", TAPE_LEN);
    std::vector<val_type> mem(tape.length());
    for (size_t i = 0; i < mem.size(); i++)
    {
        mem[i] = i;
    }

    copyFromMem(&tape, mem.begin(), mem.end());
    tape.rewind(Backward);

    for (; !tape.atEnd(); tape.move(Forward))
    {
        ASSERT_EQ(tape.read(), mem[tape.position()]);
    }
    ASSERT_EQ(tape.read(), mem[tape.position()]);
}

TEST(TapeUtils, createTape)
{
    auto tape = createTape<val_type>("TapeUtils_createTape_file0", TAPE_LEN);

    ASSERT_EQ(tape.length(), TAPE_LEN);
}

TEST(TapeUtils, createTempTape)
{
    const size_t TAPE_LEN = 1 << 8;
    auto tempTape = createTempTape<val_type>(TAPE_LEN);

    ASSERT_EQ(tempTape.length(), TAPE_LEN);
}

TEST(TapeUtils, loadLatency)
{
    std::fstream file("/tmp/cfg_tape", std::ios::in | std::ios::out);
    file << "1 2 3 4";
    file.seekg(0);

    using Time = Tape<val_type>::time_type;
    auto lat = Tape<val_type>::Latency(
        Time(1),
        Time(2),
        Time(3),
        Time(4)
    );
    ASSERT_EQ(loadLatency<val_type>(file), lat);
}

} // namespace ts