#ifndef MAZE_HPP
#define MAZE_HPP

#include <stdexcept>
#include "../common/Coordinate.hpp"
#include "../common/Direction.hpp"
#include "../common/Size.hpp"

namespace Mazemouse {

template <typename E>
concept DerivedFromEdge = std::is_base_of_v<Edge, E>;

template <typename C, DerivedFromEdge E>
class Maze {
 protected:
    Size size_;
    C* cellMatrix;
    E* horizontal_edges_;
    E* vertical_edges_;

 public:
    explicit Maze(Size size);

    /**
     * @brief Retrieves the size of this maze.
     * @return the size of this maze.
     */
    [[nodiscard]] Size size() const;

    [[nodiscard]] C& cell(const Coordinate& coordinate) const;

    [[nodiscard]] E& edge(const Coordinate& coordinate1, const Coordinate& coordinate2) const;

    [[nodiscard]] bool blocked(const Coordinate& coordinate, Direction direction) const;
};

}  // namespace Mazemouse

namespace Mazemouse {

template <typename C, DerivedFromEdge E>
Maze<C, E>::Maze(const Size size) : size_(size) {
    const auto width = size.width();
    const auto height = size.height();
    cellMatrix = new C[width * height];
    horizontal_edges_ = new E[(width - 1) * height];
    vertical_edges_ = new E[(height - 1) * width];
}

template <typename C, DerivedFromEdge E>
Size Maze<C, E>::size() const {
    return size_;
}

template <typename C, DerivedFromEdge E>
C& Maze<C, E>::cell(const Coordinate& coordinate) const {
    return cellMatrix[size_.width() * coordinate.y() + coordinate.x()];
}

template <typename C, DerivedFromEdge E>
E& Maze<C, E>::edge(const Coordinate& coordinate1, const Coordinate& coordinate2) const {
    const int x1 = coordinate1.x();
    const int y1 = coordinate1.y();
    const int x2 = coordinate2.x();
    const int y2 = coordinate2.y();
    const int deltaX = x1 - x2;
    const int deltaY = y1 - y2;
    if (deltaX == 0) {
        if (deltaY == 1) {
            return vertical_edges_[(size_.height() - 1) * x1 + y2];
        }
        if (deltaY == -1) {
            return vertical_edges_[(size_.height() - 1) * x1 + y1];
        }
    } else if (deltaY == 0) {
        if (deltaX == 1) {
            return horizontal_edges_[(size_.width() - 1) * y1 + x2];
        }
        if (deltaX == -1) {
            return horizontal_edges_[(size_.width() - 1) * y1 + x1];
        }
    }

    throw std::invalid_argument("Coordinates do not match coordinates");
}

template <typename C, DerivedFromEdge E>
bool Maze<C, E>::blocked(const Coordinate& coordinate, const Direction direction) const {
    const auto x = coordinate.x();
    const auto y = coordinate.y();

    switch (direction) {
        case Direction::UP:
            return y == 0 ? true : !edge(coordinate, { x, y - 1 }).existed();
        case Direction::RIGHT:
            return x == size_.width() - 1 ? true : !edge(coordinate, { x + 1, y }).existed();
        case Direction::DOWN:
            return y == size_.height() - 1 ? true : !edge(coordinate, { x, y + 1 }).existed();
        case Direction::LEFT:
            return x == 0 ? true : !edge(coordinate, { x - 1, y }).existed();
        default:
            return false;
    }
}

}  // namespace Mazemouse

#endif
