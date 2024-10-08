#ifndef MAZE_HPP
#define MAZE_HPP

#include <stdexcept>
#include <string>
#include <SFML/System/Vector2.hpp>
#include "Dir4.hpp"

namespace Mazemouse {

/**
 * Represents a base class for a cell in the maze.
 */
struct Cell {};

/**
 * Represents a base class for an edge between cells in the maze.
 */
struct Edge {
    /**
     * Indicates whether this edge is blocked by a wall.
     */
    mutable bool hasWall{ true };
};

template <typename C>
concept DerivedFromCell = std::is_base_of_v<Cell, C>;

template <typename E>
concept DerivedFromEdge = std::is_base_of_v<Edge, E>;

/**
 * Represents a generic square maze with cells and edges.
 *
 * @tparam S The size of the maze (number of cells along one dimension).
 * @tparam C A type derived from Cell, representing each cell in the maze.
 * @tparam E A type derived from Edge, representing each edge between cells.
 */
template <int S, DerivedFromCell C, DerivedFromEdge E>
struct Maze {
    /**
     * Array of cells in the maze.
     */
    C cells[S * S];

    /**
     * Array of edges in the maze. The first half corresponds to vertical edges,
     * and the second half corresponds to horizontal edges.
     */
    E edges[(S - 1) * S * 2];

    /**
     * Returns the index of the cell at the given coordinates.
     *
     * @param coord The coordinates of the cell.
     * @return The index of the cell in the `cells` array.
     */
    [[nodiscard]] static int cellIndex(const sf::Vector2i& coord);

    /**
     * Returns a reference to the cell at the given coordinates.
     *
     * @param coord The coordinates of the cell.
     * @return A reference to the cell.
     */
    [[nodiscard]] C& cell(const sf::Vector2i& coord);

    /**
     * Returns the index of the edge at the given coordinates and direction.
     *
     * @param coord The coordinates of the starting cell of the edge.
     * @param dir The direction of the edge.
     * @return The index of the edge in the `edges` array.
     */
    [[nodiscard]] static int edgeIndex(const sf::Vector2i& coord, Dir4 dir);

    /**
     * Returns a const reference to the edge at the given coordinates and
     * direction.
     *
     * @param coord The coordinates of the starting cell of the edge.
     * @param dir The direction of the edge.
     * @return A const reference to the edge.
     * @throws std::invalid_argument if the coordinates are out of bounds.
     */
    [[nodiscard]] const E& edge(const sf::Vector2i& coord, Dir4 dir) const;

    /**
     * Checks if the given coordinates and direction are within the maze bounds.
     *
     * @param coord The coordinates of the cell.
     * @param dir The direction to check.
     * @return True if the coordinates and direction are within bounds, false
     * otherwise.
     */
    [[nodiscard]] static bool withinBounds(const sf::Vector2i& coord, Dir4 dir);

    /**
     * Checks if the edge at the given coordinates and direction is open.
     *
     * @param coord The coordinates of the starting cell of the edge.
     * @param dir The direction of the edge.
     * @return True if the edge is open (i.e., no wall), false otherwise.
     */
    [[nodiscard]] bool isOpen(const sf::Vector2i& coord, Dir4 dir) const;
};

template <int S, DerivedFromCell C, DerivedFromEdge E>
int Maze<S, C, E>::cellIndex(const sf::Vector2i& coord) {
    return S * coord.y + coord.x;
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
C& Maze<S, C, E>::cell(const sf::Vector2i& coord) {
    return cells[cellIndex(coord)];
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
int Maze<S, C, E>::edgeIndex(const sf::Vector2i& coord, Dir4 dir) {
    const auto dirInt = static_cast<int>(dir);
    return dirInt % 2 == 0 ? (S - 1) * coord.x + coord.y - (dirInt == 0)
                           : (S - 1) * (S + coord.y) + coord.x - (dirInt == 3);
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
const E& Maze<S, C, E>::edge(const sf::Vector2i& coord, const Dir4 dir) const {
    if (!withinBounds(coord, dir)) {
        throw std::invalid_argument(
            "Maze::edge(): coord is out of range: (" + std::to_string(coord.x) +
            ", " + std::to_string(coord.y) + ") " +
            std::to_string(static_cast<int>(dir)));
    }

    return edges[edgeIndex(coord, dir)];
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
bool Maze<S, C, E>::withinBounds(const sf::Vector2i& coord, const Dir4 dir) {
    switch (dir) {
        case Dir4::Up:
            return coord.y > 0;
        case Dir4::Right:
            return coord.x < S - 1;
        case Dir4::Down:
            return coord.y < S - 1;
        case Dir4::Left:
            return coord.x > 0;
    }
    return false;
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
bool Maze<S, C, E>::isOpen(const sf::Vector2i& coord, const Dir4 dir) const {
    return withinBounds(coord, dir) && !edge(coord, dir).hasWall;
}

}  // namespace Mazemouse

#endif
