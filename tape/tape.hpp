#ifndef TAPE_HPP
#define TAPE_HPP

#include <fstream>

#include "itape.hpp"

namespace ts {

struct FileError : public std::runtime_error
{
    FileError(const std::string& what);
};

struct OutOfBorder : public std::runtime_error
{
    OutOfBorder(const std::string& what);
};

class Tape : public ITape<int32_t>
{
public:
    explicit Tape(const char* fileName);

    void moveForward() override;
    void moveBackward() override;
    void rewind() override;

    pos_type position() const override;
    pos_type length() const override;

    val_type read() const override;
    void write(const val_type& val) override;

    static pos_type cellSize();

private:
    std::fstream file_;
};

} // namespace ts

#endif // TAPE_HPP
