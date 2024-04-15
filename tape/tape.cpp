#include "tape.hpp"

namespace ts {

FileError::FileError(const std::string& what)
    : std::runtime_error("File error: " + what)
{}

OutOfBorder::OutOfBorder(const std::string& what)
    : std::runtime_error("Out of border error: " + what)
{}

Tape::Tape(const char* fileName)
    : file_(fileName, std::ios::binary | std::ios::in | std::ios::out)
{
    if (!file_.is_open())
    {
        throw FileError(std::string("File ") + fileName + " can not be open");
    }
}

void Tape::moveForward()
{
    if (position() == length() - 1)
    {
        throw OutOfBorder("End of tape");
    }

    file_.seekg(static_cast<pos_type>(file_.tellg()) + cellSize());
}

void Tape::moveBackward()
{
    if (position() == 0)
    {
        throw OutOfBorder("Begin of tape");
    }

    file_.seekg(static_cast<pos_type>(file_.tellg()) - cellSize());
}

void Tape::rewind()
{
    file_.seekg(0);
}

Tape::pos_type Tape::position() const
{
    return const_cast<std::fstream&>(file_).tellg() / cellSize();
}

Tape::pos_type Tape::length() const
{
    auto& file_ref = const_cast<std::fstream&>(file_);

    auto pos = file_ref.tellg();
    file_ref.seekg(0, std::ios_base::end);

    auto len = file_ref.tellg();
    file_ref.seekg(pos);

    return len / cellSize();
}

Tape::val_type Tape::read() const
{
    auto& file_ref = const_cast<std::fstream&>(file_);
    auto pos = file_ref.tellg();

    val_type val;
    file_ref.read(reinterpret_cast<char*>(&val), cellSize());
    file_ref.seekg(pos);

    return val;
}

void Tape::write(const val_type& val)
{
    auto pos = file_.tellg();
    file_.write(reinterpret_cast<const char*>(&val), cellSize());
    file_.seekg(pos);
}

Tape::pos_type Tape::cellSize()
{
    return sizeof(val_type);
}

} // namespace ts
