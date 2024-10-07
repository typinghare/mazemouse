#ifndef FLOOD_FILL_MOUSE_HPP
#define FLOOD_FILL_MOUSE_HPP

#include "Mouse.hpp"

namespace Mazemouse {

struct FloodFillCell : Cell {
    mutable int num_visited{ 0 };
};

template <typename C>
concept DerivedFromFloodFillCell = std::is_base_of_v<FloodFillCell, C>;

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
struct FloodFillMouse : Mouse<S, C, E> {
    void nextCycle() override;
    void moveForward(int length) override;

 protected:
    void takeOneStep(bool turned);
};

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
void FloodFillMouse<S, C, E>::nextCycle() {
    // Flood Fill Algorithm is only responsible for exploring the maze
    if (!this->exploring) {
        return;
    }

    // Check walls from the three directions and update the maze memory
    const auto updateWallMemory = [&](Direction dir) {
        const auto abs_dir = this->getAbsoluteDirection(dir);
        if (!this->maze.withinMaze(this->position, abs_dir)) {
            return;
        }

        if (this->checkWall(dir)) {
            return;
        }

        std::cout << "Unset wall: (" << this->position.x << ", "
                  << this->position.y << ") " << to_int(abs_dir) << " | "
                  << this->maze.vEdgeIndex(this->position, abs_dir) << " | "
                  << this->maze.hEdgeIndex(this->position, abs_dir)
                  << std::endl;
        this->maze.edge(this->position, abs_dir).hasWall = false;
    };

    updateWallMemory(Direction::UP);
    updateWallMemory(Direction::RIGHT);
    updateWallMemory(Direction::LEFT);

    takeOneStep(false);
}

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
void FloodFillMouse<S, C, E>::moveForward(int length) {
    if (this->exploring) {
        this->maze.edge(this->position, this->orientation).visited = true;
    }

    Mouse<S, C, E>::moveForward(1);
}

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
void FloodFillMouse<S, C, E>::takeOneStep(const bool turned) {
    // Absolute directions
    const auto forward = this->orientation;
    const auto rightward = this->getAbsoluteDirection(Direction::RIGHT);
    const auto backward = this->getAbsoluteDirection(Direction::DOWN);

    auto can_move_forward = this->maze.withinMaze(this->position, forward) &&
                            !this->maze.edge(this->position, forward).hasWall;
    auto can_move_rightward =
        this->maze.withinMaze(this->position, rightward) &&
        !this->maze.edge(this->position, rightward).hasWall;
    auto can_move_backward = this->maze.withinMaze(this->position, backward) &&
                             !this->maze.edge(this->position, backward).hasWall;
    auto forward_visited =
        can_move_forward ? this->maze.edge(this->position, forward).visited
                         : -1;
    auto rightward_visited =
        can_move_rightward ? this->maze.edge(this->position, rightward).visited
                           : -1;
    auto backward_visited =
        can_move_backward ? this->maze.edge(this->position, backward).visited
                          : -1;

    // Priority:
    // 1. UNVISITED > VISITED
    // 2. FORWARD > RIGHT > TURN BACK
    if (forward_visited == 0) {
        return this->moveForward(1);
    }

    if (rightward_visited == 0) {
        this->turnTo(rightward);
        return this->moveForward(1);
    }

    if (backward_visited == 0) {
        this->turnTo(backward);
        return this->moveForward(1);
    }

    // If all possible directions are either visited or blocked, move to the
    // least visited option
    if (can_move_forward) {
        return this->moveForward(1);  // Default move forward
    }

    if (!turned) {
        // Turn around to prevent immediate backtracking in loops
        this->turnTo(backward);
        takeOneStep(true);  // Recursive call with a turn flag set to true
    }
}

}  // namespace Mazemouse

#endif
