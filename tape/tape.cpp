#include "tape.hpp"

namespace ts {

FileError::FileError(const std::string& what)
    : std::runtime_error("File error: " + what)
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
    file_.seekg(getPosition() + cellSize());
}

void Tape::moveBackward()
{
    file_.seekg(getPosition() - cellSize());
}

void Tape::rewind()
{
    file_.seekg(0);
}

Tape::pos_type Tape::getPosition() const
{
    return const_cast<std::fstream&>(file_).tellg();
}

Tape::val_type Tape::read() const
{
    auto pos = getPosition();

    val_type val;
    const_cast<std::fstream&>(file_).read(reinterpret_cast<char*>(&val), cellSize());

    const_cast<std::fstream&>(file_).seekg(pos);
    return val;
}

void Tape::write(const val_type& val)
{
    auto pos = getPosition();

    file_.write(reinterpret_cast<const char*>(&val), cellSize());

    file_.seekg(pos);
}

Tape::pos_type Tape::cellSize()
{
    return sizeof(val_type);
}

} // namespace ts
