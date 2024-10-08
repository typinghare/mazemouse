#ifndef FLOOD_FILL_MOUSE_HPP
#define FLOOD_FILL_MOUSE_HPP

#include <stack>
#include "Mouse.hpp"

namespace Mazemouse {

struct FloodFillCell : Cell {
    mutable int num_visited{ 0 };
};

template <typename C>
concept DerivedFromFloodFillCell = std::is_base_of_v<FloodFillCell, C>;

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
struct FloodFillMouse : Mouse<S, C, E> {
    FloodFillMouse(
        const sf::Vector2i startingPosition, const Dir4 startingOrientation) :
        Mouse<S, C, E>(startingPosition, startingOrientation){};

    void nextCycle() override;

    void moveForward(int length) override;

 protected:
    void updateWallMemory();

    void exploreNext();

    bool hasArrivedAtFinish();

    bool hasArrivedAtStarting();

    void returnAlongOriginalRoute();

    bool canMove(Dir4 absolute_dir);

    C& getCellOn(Dir4 absolute_dir);

    std::vector<Dir4> stack{};
};

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
void FloodFillMouse<S, C, E>::nextCycle() {
    if (this->state == MouseState::Exploring) {
        if (hasArrivedAtFinish()) {
            this->state = MouseState::ReturningToStart;
            return;
        }

        updateWallMemory();
        exploreNext();
    } else if (this->state == MouseState::ReturningToStart) {
        if (hasArrivedAtStarting()) {
            this->state = MouseState::RushingToFinish;
            return;
        }

        returnAlongOriginalRoute();
    }
}

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
void FloodFillMouse<S, C, E>::moveForward(int length) {
    Mouse<S, C, E>::moveForward(length);

    if (this->state == MouseState::Exploring) {
        ++this->maze.cell(this->position).num_visited;
    }
}

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
void FloodFillMouse<S, C, E>::updateWallMemory() {
    if (this->state != MouseState::Exploring) {
        return;
    }

    const auto updateWallMemory = [&](Dir4 dir) {
        const auto absolute_dir = this->getAbsoluteDir(dir);
        if (!this->maze.withinBounds(this->position, absolute_dir)) {
            return;
        }

        if (this->checkWall(dir)) {
            return;
        }

        this->maze.edge(this->position, absolute_dir).hasWall = false;
    };

    updateWallMemory(Dir4::Up);
    updateWallMemory(Dir4::Right);
    updateWallMemory(Dir4::Left);
}

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
void FloodFillMouse<S, C, E>::exploreNext() {
    // Four absolute directions
    Dir4 dirs[4] = { this->orientation };
    for (int i = 1; i < 4; i++) {
        dirs[i] = dirs[i - 1] + Dir4::Right;
    }

    // Checks if the mouse can move towards each direction
    bool moveable[4] = {};
    for (int i = 0; i < 4; i++) {
        moveable[i] = canMove(dirs[i]);
    }

    int num_visited[4] = {};
    for (int i = 0; i < 4; i++) {
        num_visited[i] = moveable[i] ? getCellOn(dirs[i]).num_visited : INT_MAX;
    }

    auto next_absolute_dir = dirs[0];
    auto min_num_visited = num_visited[0];
    for (int i = 1; i < 4; i++) {
        if (num_visited[i] < min_num_visited) {
            next_absolute_dir = dirs[i];
            min_num_visited = num_visited[i];
        }
    }

    // Handling stack
    if (!stack.empty()) {
        const auto previous_dir = stack.back();
        if (previous_dir - next_absolute_dir == Dir4::Down) {
            stack.pop_back();
        } else {
            stack.push_back(next_absolute_dir);
        }
    } else {
        stack.push_back(next_absolute_dir);
    }

    this->turn(next_absolute_dir);
    this->moveForward(1);
}

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
bool FloodFillMouse<S, C, E>::hasArrivedAtFinish() {
    const int a = S / 2, b = a - 1;

    return (this->position.x == a || this->position.x == b) &&
           (this->position.y == a || this->position.y == b);
}

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
bool FloodFillMouse<S, C, E>::hasArrivedAtStarting() {
    return this->position.x == 0 && this->position.y == S - 1;
}

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
void FloodFillMouse<S, C, E>::returnAlongOriginalRoute() {
    if (stack.empty()) {
        return;
    }

    const auto opposite_absolute_dir = stack.back();
    stack.pop_back();
    this->turn(opposite_absolute_dir + Dir4::Down);
    moveForward(1);
}

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
bool FloodFillMouse<S, C, E>::canMove(const Dir4 absolute_dir) {
    return this->maze.withinBounds(this->position, absolute_dir) &&
           !this->maze.edge(this->position, absolute_dir).hasWall;
}

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
C& FloodFillMouse<S, C, E>::getCellOn(const Dir4 absolute_dir) {
    return this->maze.cell(this->position + get_vector(absolute_dir));
}

}  // namespace Mazemouse

#endif
