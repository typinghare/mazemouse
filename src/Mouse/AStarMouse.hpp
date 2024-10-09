#ifndef A_STAR_MOUSE_HPP
#define A_STAR_MOUSE_HPP
#include "FloodFillMouse.hpp"

namespace Mazemouse {

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
class AStarMouse : public FloodFillMouse<S, C, E> {
 public:
    AStarMouse(
        const sf::Vector2i startingPosition, const Dir4 startingOrientation) :
        FloodFillMouse<S, C, E>(startingPosition, startingOrientation){};

    void nextExploringCycle() override;

    void nextRushingCycle() override;

 protected:
    std::vector<Dir4> vector{};

    int total_step{ 0 };
};

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
void AStarMouse<S, C, E>::nextExploringCycle() {
    FloodFillMouse<S, C, E>::nextExploringCycle();

    if (this->state == MouseState::ReturningToStart && vector.empty()) {
        // Copy the stack
        vector = this->stack;
    }
}

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
void AStarMouse<S, C, E>::nextRushingCycle() {
    if (this->hasArrivedAtFinish()) {
        this->state = MouseState::Stopped;
        return;
    }

    const auto absolute_dir = vector[total_step++];
    this->turn(absolute_dir);

    int step = 1;
    while (total_step < vector.size() && vector[total_step] == absolute_dir) {
        step++;
        total_step++;
    }
    this->moveForward(step);
}

}  // namespace Mazemouse

#endif
