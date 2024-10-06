#ifndef DIRECTION_HPP
#define DIRECTION_HPP

#include <stdexcept>
#include <SFML/System/Vector2.hpp>

namespace Mazemouse {

enum class Direction : int {
    UP = 0b00,
    RIGHT = 0b01,
    DOWN = 0b10,
    LEFT = 0b11
};

const sf::Vector2i DirectionVectors[4] = { sf::Vector2i{ 0, -1 },
                                           sf::Vector2i{ 1, 0 },
                                           sf::Vector2i{ 0, 1 },
                                           sf::Vector2i{ -1, 0 } };

inline Direction getDirectionByVector(const sf::Vector2i& vector) {
    if (vector.x == 0) {
        if (vector.y == -1) {
            return Direction::UP;
        }
        if (vector.y == 1) {
            return Direction::DOWN;
        }
    }
    if (vector.y == 0) {
        if (vector.x == -1) {
            return Direction::LEFT;
        }
        if (vector.x == 1) {
            return Direction::RIGHT;
        }
    }

    throw std::invalid_argument("Invalid vector coordinates.");
}

}  // namespace Mazemouse

#endif
