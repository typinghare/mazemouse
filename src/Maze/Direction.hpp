#ifndef DIRECTION_HPP
#define DIRECTION_HPP

namespace Mazemouse {

enum class Direction : int {
    UP = 0b00,
    RIGHT = 0b01,
    DOWN = 0b10,
    LEFT = 0b11
};

const sf::Vector2i DirectionVectors[4] = {
    { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 }
};

}  // namespace Mazemouse

#endif
