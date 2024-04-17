#ifndef ITAPE_HPP
#define ITAPE_HPP

namespace ts {

template <typename T>
class ITape
{
public:
    using pos_type = size_t;
    using val_type = T;

    enum class MoveDirection { Forward, Backward };

    ITape() = default;

    ITape(ITape&&) = default;
    ITape& operator=(ITape&&) = default;

    ITape(const ITape&) = delete;
    ITape& operator=(const ITape&) = delete;

    virtual ~ITape() = default;

    virtual void move(MoveDirection) = 0;
    virtual void rewind(MoveDirection) = 0;

    virtual pos_type position() const = 0;
    virtual pos_type length() const = 0;

    virtual bool atBegin() const = 0;
    virtual bool atEnd() const = 0;
    virtual bool atEdge(MoveDirection) const = 0;

    virtual val_type read() const = 0;
    virtual void write(const val_type&) = 0;
};

} // namespace ts

#endif // ITAPE_HPP