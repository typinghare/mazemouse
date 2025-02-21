#ifndef MOUSE_HPP
#define MOUSE_HPP

#include "../Maze/Maze.hpp"

namespace Mazemouse {

constexpr auto MOUSE_STARTING_ORIENTATION = Dir4::Up;

enum class MouseState : int {
    // Stop still
    Stopped,

    // Actively exploring the maze
    Exploring,

    // Navigating back to the starting cell
    ReturningToStart,

    // Moving quickly towards the finishing cells
    RushingToFinish
};

/**
 * @brief Interface defining functions for hardware-level mouse interactions.
 *
 * This interface serves as an abstraction layer for interacting with the
 * hardware APIs specific to a mouse device. Implementations of this interface
 * should provide concrete definitions for hardware control and sensor data
 * retrieval operations, allowing for easier testing and integration of hardware
 * components.
 */
struct MouseHardwareInterface {
    virtual ~MouseHardwareInterface() = default;

    /**
     * @brief Checks for the presence of a wall in a specified direction.
     *
     * This function should be implemented to return true if there is a wall in
     * the specified direction (front, right, or left), and false otherwise.
     * The direction is relative to the mouse's current orientation, which
     * does not include a backward direction.
     *
     * @param dir The direction to check for a wall, limited to front, right, or
     * left.
     * @return true if there is a wall in the specified direction, false
     * otherwise.
     */
    virtual bool hardwareCheckWall(Dir4 dir) = 0;

    /**
     * @brief Moves the mouse forward by a specified length.
     *
     * This function should be implemented to move the mouse forward in its
     * current orientation. The length parameter indicates how far to move.
     *
     * @param step The distance to move forward.
     */
    virtual void hardwareMoveForward(int step) = 0;

    /**
     * @brief Turns the mouse relative to its current direction.
     *
     * This function should be implemented to change the orientation of the
     * mouse based on the specified relative direction.
     *
     * @param relative_dir The direction to turn relative to the current
     * orientation.
     */
    virtual void hardwareTurn(Dir4 relative_dir) = 0;
};

/**
 * @brief Represents the mouse that explores a maze.
 *
 * This struct encapsulates the state and behavior of the mouse as it
 * navigates through a maze. The mouse maintains a representation of the maze
 * in its memory and tracks its current position, orientation, and exploration
 * status.
 */
template <int S, DerivedFromCell C, DerivedFromEdge E>
struct Mouse : MouseHardwareInterface {
    /**
     * @brief Represents the maze in the mouse's memory.
     *
     * This member stores the maze structure, where all edges have their hasWall
     * property initialized to true at the beginning. As the mouse explores the
     * maze, the hasWall property of edges will be updated to reflect the
     * actual configuration of the maze.
     */
    Maze<S, C, E> maze;

    Vector2 startingPosition;

    Dir4 startingOrientation;

    /**
     * @brief The current position of the mouse in the maze.
     *
     * This member holds the current coordinates of the mouse within the
     * maze, represented as a 2D vector.
     */
    Vector2 position;

    /**
     * @brief The orientation the mouse is currently facing.
     *
     * This member indicates the direction the mouse is facing, which is
     * defined using the Dir4 enumeration. The initial orientation is set to
     * the defined starting orientation (MOUSE_STARTING_ORIENTATION).
     */
    Dir4 orientation;

    /**
     * @brief The state of the mouse.
     */
    MouseState state{ MouseState::Stopped };

    Mouse(Vector2 startingPosition, Dir4 startingOrientation);

    /**
     * @brief Calculates the absolute direction based on the current
     * orientation.
     *
     * This method takes a relative direction and calculates the corresponding
     * absolute direction based on the mouse's current orientation.
     *
     * @param relative_dir The relative direction to calculate the absolute
     * direction from.
     * @return The absolute direction resulting from the addition of the current
     * orientation and the relative direction.
     */
    [[nodiscard]] Dir4 getAbsoluteDir(Dir4 relative_dir) const;

    /**
     * @brief Calculates the relative direction based on the current
     * orientation.
     *
     * This method takes an absolute direction and computes the corresponding
     * relative direction with respect to the mouse's current orientation.
     *
     * @param absolute_dir The absolute direction to calculate the relative
     * direction from.
     * @return The relative direction resulting from the subtraction of the
     * current orientation from the absolute direction.
     */
    [[nodiscard]] Dir4 getRelativeDir(Dir4 absolute_dir) const;

    /**
     * @brief Checks if there is a wall in a given direction.
     *
     * This method uses the hardware interface to determine if there is a wall
     * in the specified direction relative to the mouse's current orientation.
     *
     * @param dir The direction to check for a wall, relative to the mouse's
     * current orientation.
     * @return true if there is a wall in the specified direction, false
     * otherwise.
     */
    virtual bool checkWall(Dir4 dir);

    /**
     * @brief Moves the mouse forward by a given distance.
     *
     * This method commands the mouse to move forward by the specified number
     * of units and updates the mouse's position in the maze based on its
     * current orientation.
     *
     * @param length The distance to move forward.
     */
    virtual void moveForward(int length);

    /**
     * @brief Turns the mouse to face the target orientation.
     *
     * This method adjusts the mouse's current orientation to match the target
     * orientation and commands the hardware to turn accordingly.
     *
     * @param target_orientation The target orientation the mouse should turn
     * to.
     */
    virtual void turn(Dir4 target_orientation);

    /**
     * @brief Executes the next step in the exploration process.
     *
     * This pure virtual method is intended to advance the mouse's state while
     * it is actively exploring the maze. Derived classes must implement the
     * logic for the next step in exploration.
     */
    virtual void nextExploringCycle() = 0;

    /**
     * @brief Executes the next step in the rushing process.
     *
     * This pure virtual method is intended to advance the mouse's state when
     * rushing towards the finish. Derived classes must implement the logic for
     * the next step during the rushing phase.
     */
    virtual void nextRushingCycle() = 0;

    /**
     * @brief
     */
    virtual void resetRushingState();
};

template <int S, DerivedFromCell C, DerivedFromEdge E>
Mouse<S, C, E>::Mouse(
    const Vector2 startingPosition, const Dir4 startingOrientation) :
    startingPosition{ startingPosition },
    startingOrientation{ startingOrientation }, position{ startingPosition },
    orientation(startingOrientation) {}

template <int S, DerivedFromCell C, DerivedFromEdge E>
Dir4 Mouse<S, C, E>::getAbsoluteDir(const Dir4 relative_dir) const {
    return orientation + relative_dir;
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
Dir4 Mouse<S, C, E>::getRelativeDir(const Dir4 absolute_dir) const {
    return absolute_dir - orientation;
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
bool Mouse<S, C, E>::checkWall(const Dir4 dir) {
    return hardwareCheckWall(dir);
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
void Mouse<S, C, E>::moveForward(const int length) {
    hardwareMoveForward(length);

    switch (orientation) {
        case Dir4::Up:
            position.y -= length;
            break;
        case Dir4::Right:
            position.x += length;
            break;
        case Dir4::Down:
            position.y += length;
            break;
        case Dir4::Left:
            position.x -= length;
    }
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
void Mouse<S, C, E>::turn(const Dir4 target_orientation) {
    orientation = target_orientation;

    return hardwareTurn(getRelativeDir(orientation));
}

template <int S, DerivedFromCell C, DerivedFromEdge E>
void Mouse<S, C, E>::resetRushingState() {
    this->position = startingPosition;
    this->orientation = startingOrientation;
    this->state = MouseState::RushingToFinish;
}

}  // namespace Mazemouse

#endif
