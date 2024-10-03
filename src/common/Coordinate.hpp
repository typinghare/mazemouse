#ifndef COORDINATE_HPP
#define COORDINATE_HPP

namespace Mazemouse {

class Coordinate {
    int x_;
    int y_;

 public:
    template <typename X = int, typename Y = int>
    Coordinate(X x, Y y);

    [[nodiscard]] int x() const;

    [[nodiscard]] int y() const;
};

template <typename X, typename Y>
Coordinate::Coordinate(X x, Y y) : x_{ static_cast<int>(x) }, y_{ static_cast<int>(y) } {}

inline int Coordinate::x() const { return x_; }

inline int Coordinate::y() const { return y_; }

}  // namespace Mazemouse

#endif
