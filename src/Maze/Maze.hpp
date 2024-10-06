#ifndef MAZE_HPP
#define MAZE_HPP

#include <iostream>
#include <stdexcept>
#include <SFML/System/Vector2.hpp>
#include "Cell.hpp"
#include "Direction.hpp"
#include "Edge.hpp"

namespace Mazemouse {

template <typename C>
concept DerivedFromCell = std::is_base_of_v<Cell, C>;

template <typename E>
concept DerivedFromEdge = std::is_base_of_v<Edge, E>;

template <int S, DerivedFromCell C, DerivedFromEdge E>
struct Maze {
    C cells[S * S];
    E v_edges[(S - 1) * S];
    E h_edges[(S - 1) * S];

    [[nodiscard]] C& cell(const sf::Vector2i& coord);

    [[nodiscard]] const E& edge(const sf::Vector2i& coord, Direction dir) const;

    [[nodiscard]] static int
    vEdgeIndex(const sf::Vector2i& coord, Direction dir);

    [[nodiscard]] static int
    hEdgeIndex(const sf::Vector2i& coord, Direction dir);

    [[nodiscard]] static bool
    checkBound(const sf::Vector2i& coord, Direction dir);

    [[nodiscard]] bool
    isEdgeOpen(const sf::Vector2i& coord, Direction dir) const;
};

template <int S, DerivedFromCell C, DerivedFromEdge E>
C& Maze<S, C, E>::cell(const sf::Vector2i& coord) {
    return cells[S * coord.y + coord.x];
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
const E&
Maze<S, C, E>::edge(const sf::Vector2i& coord, const Direction dir) const {
    if (!checkBound(coord, dir)) {
        throw std::invalid_argument(
            "Maze::edge(): coord is out of range: (" + std::to_string(coord.x) +
            ", " + std::to_string(coord.y) + ") " +
            std::to_string(static_cast<int>(dir)));
    }

    switch (dir) {
        case Direction::UP:
        case Direction::DOWN:
            return v_edges[vEdgeIndex(coord, dir)];
        case Direction::RIGHT:
        case Direction::LEFT:
            return h_edges[hEdgeIndex(coord, dir)];
    }
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
int Maze<S, C, E>::vEdgeIndex(const sf::Vector2i& coord, const Direction dir) {
    return (S - 1) * coord.x + coord.y - static_cast<int>(dir == Direction::UP);
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
int Maze<S, C, E>::hEdgeIndex(const sf::Vector2i& coord, const Direction dir) {
    return (S - 1) * coord.y + coord.x -
           static_cast<int>(dir == Direction::LEFT);
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
bool Maze<S, C, E>::checkBound(const sf::Vector2i& coord, const Direction dir) {
    switch (dir) {
        case Direction::UP:
            return coord.y > 0;
        case Direction::RIGHT:
            return coord.x < S - 1;
        case Direction::DOWN:
            return coord.y < S - 1;
        case Direction::LEFT:
            return coord.x > 0;
    }
    return false;
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
bool Maze<S, C, E>::isEdgeOpen(
    const sf::Vector2i& coord, const Direction dir) const {
    if (!checkBound(coord, dir)) {
        return false;
    }

    auto e = edge(coord, dir);
    return !edge(coord, dir).hasWall;
}

}  // namespace Mazemouse

#endif
