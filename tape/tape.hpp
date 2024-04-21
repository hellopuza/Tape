#ifndef TAPE_TAPE_HPP
#define TAPE_TAPE_HPP

#include "itape.hpp"

#include <chrono>
#include <fstream>
#include <thread>

namespace ts {

struct FileError : public std::runtime_error
{
    FileError(const std::string& what)
        : std::runtime_error("File error: " + what)
    {}
};

struct OutOfBorder : public std::runtime_error
{
    OutOfBorder(const std::string& what)
        : std::runtime_error("Out of border error: " + what)
    {}
};

template <typename T, typename Time = std::chrono::milliseconds>
class Tape : public ITape<T>
{
public:
    using pos_type = typename ITape<T>::pos_type;
    using val_type = typename ITape<T>::val_type;
    using MoveDirection = typename ITape<T>::MoveDirection;
    using time_type = Time;

    struct Latency
    {
        Time read;
        Time write;
        Time move;
        Time rewind;

        bool operator==(const Latency& rhs) const = default;
    };

    explicit Tape(const char* fileName, const Latency& latency = Latency())
        : file_(fileName, std::ios::binary | std::ios::in | std::ios::out)
        , length_((file_.seekg(0, std::ios_base::end), file_.tellg() / cellSize()))
        , latency_(latency)
    {
        if (!file_.is_open())
        {
            throw FileError(std::string("File ") + fileName + " can not be open");
        }

        file_.seekg(0);
    }

    void move(MoveDirection dir) override
    {
        std::this_thread::sleep_for(latency_.move);
        switch (dir)
        {
        case MoveDirection::Forward:
            if (atEnd()) throw OutOfBorder("End of tape");
            file_.seekg(cellSize(), std::ios_base::cur);
            break;

        case MoveDirection::Backward:
            if (atBegin()) throw OutOfBorder("Begin of tape");
            file_.seekg(-cellSize(), std::ios_base::cur);
            break;
        }
    }

    void rewind(MoveDirection dir) override
    {
        std::this_thread::sleep_for(latency_.rewind);
        switch (dir)
        {
        case MoveDirection::Forward:
            file_.seekg(-cellSize(), std::ios_base::end);
            break;
        case MoveDirection::Backward:
            file_.seekg(0);
            break;
        }
    }

    pos_type position() const override
    {
        return const_cast<std::fstream&>(file_).tellg() / cellSize();
    }

    pos_type length() const override
    {
        return length_;
    }

    bool atBegin() const override
    {
        return position() == 0;
    }

    bool atEnd() const override
    {
        return position() == length() - 1;
    }

    bool atEdge(MoveDirection dir) const override
    {
        switch (dir)
        {
        case MoveDirection::Forward:
            return atEnd();
        case MoveDirection::Backward:
            return atBegin();
        }
        return false;
    }

    val_type read() const override
    {
        std::this_thread::sleep_for(latency_.read);

        auto& file_ref = const_cast<std::fstream&>(file_);
        auto pos = file_ref.tellg();

        val_type val;
        file_ref.read(reinterpret_cast<char*>(&val), cellSize());
        file_ref.seekg(pos);

        return val;
    }

    void write(const val_type& val) override
    {
        std::this_thread::sleep_for(latency_.write);

        auto pos = file_.tellg();
        file_.write(reinterpret_cast<const char*>(&val), cellSize());
        file_.seekg(pos);
    }

    static pos_type cellSize()
    {
        return sizeof(val_type);
    }

private:
    std::fstream file_;
    pos_type length_;
    Latency latency_;
};

} // namespace ts

#endif // TAPE_TAPE_HPP