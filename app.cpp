#include <sorter/sorter.hpp>

#include <iostream>

using val_type = int32_t;

constexpr size_t DEFAULT_MEMSIZE = 1 << 16;

auto getLatency(const char* filename)
{
    std::ifstream file(filename, std::ios::in);
    if (!file.is_open())
    {
        std::cerr << "Configure file " << filename << " can not be open - using zero latency\n";
        return ts::Tape<val_type>::Latency();
    }
    return ts::loadLatency<val_type>(file);
}

int main(int argc, char** argv)
{
    if (!((argc == 3) || (argc == 4)))
    {
        std::cerr << "Usage: " << argv[0] << " src-file dst-file [MEMSIZE]\n";
        return 1;
    }

    auto lat = getLatency("tape.cfg");

    ts::Tape<val_type> srcTape(argv[1], lat);

    ts::createFile(argv[2], srcTape.length() * sizeof(val_type));
    ts::Tape<val_type> dstTape(argv[2], lat);

    size_t memsize = DEFAULT_MEMSIZE;
    if (argc == 4)
    {
        char* end = nullptr;
        memsize = std::strtoul(argv[3], &end, 10);
    }

    ts::Sorter<val_type> sorter(memsize);
    sorter.sort(&srcTape, &dstTape, ts::createTempTape<val_type>, std::less<val_type>());

    return 0;
}