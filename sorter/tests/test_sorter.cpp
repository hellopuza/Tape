#include <gtest/gtest.h>

#include <sorter.hpp>

namespace ts {

constexpr size_t MEMSIZE = 1 << 10;

TEST(Sorter, creation) {
    Sorter sorter(MEMSIZE);
}

} // namespace ts
