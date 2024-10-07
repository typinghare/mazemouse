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
    FloodFillMouse() : Mouse<S, C, E>() {
        ++this->maze.cell(this->position).num_visited;
    }

    void nextCycle() override;

    void moveForward(int length) override;

 protected:
    void takeOneStep();

    bool canMove(Direction abs_dir);

    C& getCellOn(Direction abs_dir);
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

    takeOneStep();
}

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
void FloodFillMouse<S, C, E>::moveForward(int length) {
    if (this->exploring) {
        this->maze.edge(this->position, this->orientation).visited = true;
    }

    Mouse<S, C, E>::moveForward(1);

    if (this->exploring) {
        ++this->maze.cell(this->position).num_visited;
    }
}

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
void FloodFillMouse<S, C, E>::takeOneStep() {
    // Absolute directions
    const auto forward = this->orientation;
    const auto rightward = this->getAbsoluteDirection(Direction::RIGHT);
    const auto backward = this->getAbsoluteDirection(Direction::DOWN);
    const auto leftward = this->getAbsoluteDirection(Direction::LEFT);

    auto can_move_forward = canMove(forward);
    auto can_move_rightward = canMove(rightward);
    auto can_move_backward = canMove(backward);
    auto can_move_leftward = canMove(leftward);
    auto forward_num_visited =
        can_move_forward ? getCellOn(forward).num_visited : INT_MAX;
    auto rightward_num_visited =
        can_move_rightward ? getCellOn(rightward).num_visited : INT_MAX;
    auto backward_num_visited =
        can_move_backward ? getCellOn(backward).num_visited : INT_MAX;
    auto leftward_num_visited =
        can_move_leftward ? getCellOn(leftward).num_visited : INT_MAX;

    auto next_abs_dir = this->orientation;
    auto max = forward_num_visited;
    if (rightward_num_visited < max) {
        next_abs_dir = rightward;
        max = rightward_num_visited;
    }
    if (backward_num_visited < max) {
        next_abs_dir = backward;
        max = backward_num_visited;
    }
    if (leftward_num_visited < max) {
        next_abs_dir = leftward;
    }

    this->turnTo(next_abs_dir);
    this->moveForward(1);
}

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
bool FloodFillMouse<S, C, E>::canMove(Direction abs_dir) {
    return this->maze.withinMaze(this->position, abs_dir) &&
           !this->maze.edge(this->position, abs_dir).hasWall;
}

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
C& FloodFillMouse<S, C, E>::getCellOn(const Direction abs_dir) {
    return this->maze.cell(this->position + DirectionVectors[to_int(abs_dir)]);
}


}  // namespace Mazemouse

#endif
