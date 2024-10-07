#ifndef MOUSE_HPP
#define MOUSE_HPP

#include "../Maze.hpp"

namespace Mazemouse {

constexpr auto MOUSE_STARTING_POSITION_X = 0;
constexpr auto MOUSE_STARTING_POSITION_Y = 15;
constexpr auto MOUSE_STARTING_ORIENTATION = Direction::UP;

/**
 * The enumeration of a steering operation.
 */
enum class STEERING : int {
    STAY_STILL = 0,
    CLOCKWISE = 1,
    U_TURN = 2,
    COUNTERCLOCKWISE = 3
};

/**
 * Represents a micromouse running in a maze.
 * @tparam S The side length of the maze.
 * @tparam C The cell type.
 * @tparam E The edge type.
 */
template <int S, DerivedFromCell C, DerivedFromEdge E>
struct Mouse {
    /**
     * Represents the maze in the mouse memory. The hasWall property of all
     * edges are set to at the beginning. As the mouse exploring the maze, the
     * hasWall property will be unset.
     */
    Maze<S, C, E> maze;

    /**
     * The current position.
     */
    sf::Vector2i position{ MOUSE_STARTING_POSITION_X,
                           MOUSE_STARTING_POSITION_Y };

    /**
     * The orientation this mouse is facing.
     */
    Direction orientation{ Direction::UP };

    /**
     * Whether the mouse is exploring the maze.
     */
    bool exploring { true };

    virtual ~Mouse() = default;

    /**
     * Retrieves the steering based on the target orientation and current
     * orientation.
     * @param target_orientation The target orientation.
     * @return
     */
    [[nodiscard]] STEERING getSteering(Direction target_orientation) const;

    [[nodiscard]] Direction getAbsoluteDirection(Direction dir) const;

    [[nodiscard]] Direction getRelativeDirection(Direction abs_dir) const;

    /**
     * Checks if there's a wall in a certain direction.
     * @param dir The direction.
     * @return
     */
    virtual bool checkWall(Direction dir) = 0;

    /**
     * The mouse moves forward based on the current orientation.
     * @param length The length to move until it stops.
     */
    virtual void moveForward(int length);

    virtual void turnTo(Direction target_orientation);

    /**
     * The mouse steers.
     * @param steering
     */
    virtual void turn(STEERING steering) = 0;

    virtual void nextCycle() = 0;
};

template <int S, DerivedFromCell C, DerivedFromEdge E>
STEERING Mouse<S, C, E>::getSteering(const Direction target_orientation) const {
    const auto steering =
        (to_int(target_orientation) - to_int(orientation) + 4) % 4;
    return static_cast<STEERING>(steering);
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
Direction Mouse<S, C, E>::getAbsoluteDirection(const Direction dir) const {
    const auto abs_dir = (to_int(dir) + to_int(orientation)) % 4;
    return static_cast<Direction>(abs_dir);
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
Direction Mouse<S, C, E>::getRelativeDirection(const Direction abs_dir) const {
    const auto dir = (to_int(abs_dir) - to_int(orientation) + 4) % 4;
    return static_cast<Direction>(dir);
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
void Mouse<S, C, E>::moveForward(const int length) {
    switch (orientation) {
        case Direction::UP:
            position.y -= length;
            break;
        case Direction::RIGHT:
            position.x += length;
            break;
        case Direction::DOWN:
            position.y += length;
            break;
        case Direction::LEFT:
            position.x -= length;
    }
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
void Mouse<S, C, E>::turnTo(const Direction target_orientation) {
    orientation = target_orientation;
    turn(getSteering(target_orientation));
}

}  // namespace Mazemouse

#endif
