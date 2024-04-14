#ifndef ITAPE_HPP
#define ITAPE_HPP

namespace ts {

template <typename T>
class ITape
{
public:
    using pos_type = size_t;
    using val_type = T;

    ITape() = default;

    ITape(const ITape&) = delete;
    ITape& operator=(const ITape&) = delete;

    virtual ~ITape() = default;

    virtual void moveForward() = 0;
    virtual void moveBackward() = 0;
    virtual void rewind() = 0;

    virtual pos_type getPosition() const = 0;

    virtual val_type read() const = 0;
    virtual void write(const val_type&) = 0;
};

} // namespace ts

#endif // ITAPE_HPP
