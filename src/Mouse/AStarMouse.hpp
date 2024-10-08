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

    void nextCycle() override;

 protected:
    std::vector<Dir4> vector{};
    int step{ 0 };
};

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
void AStarMouse<S, C, E>::nextCycle() {
    FloodFillMouse<S, C, E>::nextCycle();

    if (this->state == MouseState::ReturningToStart && vector.empty()) {
        vector = this->stack;
        return;
    }

    if (this->state == MouseState::RushingToFinish) {
        if (this->hasArrivedAtFinish()) {
            this->state = MouseState::STOPPED;
            return;
        }

        const auto absolute_dir = vector[step++];
        this->turn(absolute_dir);
        this->moveForward(1);
    }
}

}  // namespace Mazemouse

#endif
