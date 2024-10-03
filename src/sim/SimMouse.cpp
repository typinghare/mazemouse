#include "SimMouse.hpp"

#include <iostream>

namespace Mazemouse {

SimMouse::SimMouse(SimGame* sim_game) : SimGamePlugin(sim_game) {
    teleport({ SIM_MOUSE_STARTING_POSITION_X, SIM_MOUSE_STARTING_POSITION_Y });
    destination_ = getNextPosition();
}

void SimMouse::teleport(const Coordinate& position) {
    position_ = position;
    position_pixel_ = { (0.5f + static_cast<float>(position.x())) * SIM_CELL_SIDE_LENGTH_PIXEL,
                        (0.5f + static_cast<float>(position.y())) * SIM_CELL_SIDE_LENGTH_PIXEL };
}

Coordinate SimMouse::getNextPosition() const {
    const auto maze = game_->getMaze();
    const auto x = position_.x();
    const auto y = position_.y();

    std::cout << "(" << x << ", " << y << ")" << std::endl;
    if (!maze.blocked(position_, Direction::UP) && x == 0) {
        return { x, y - 1 };
    }

    if (!maze.blocked(position_, Direction::RIGHT) && y == 0) {
        return { x + 1, y };
    }

    if (!maze.blocked(position_, Direction::DOWN) && x == SIM_MAZE_SIDE_LENGTH - 1) {
        return { x, y + 1 };
    }

    if (!maze.blocked(position_, Direction::LEFT) && y == SIM_MAZE_SIDE_LENGTH - 1) {
        return { x - 1, y };
    }

    return position_;
}

void SimMouse::draw(sf::RenderTexture& maze_render_texture) {
    static const auto COLOR = sf::Color::Black;

    auto circle = sf::CircleShape(SIM_MOUSE_RADIUS);
    circle.setFillColor(COLOR);
    circle.setOrigin(SIM_MOUSE_RADIUS, SIM_MOUSE_RADIUS);
    circle.setPosition(position_pixel_.x, position_pixel_.y);

    maze_render_texture.draw(circle);
}

void SimMouse::update(const unsigned dt) {
    const auto deltaX = destination_.x() - position_.x();
    const auto deltaY = destination_.y() - position_.y();

    if (deltaX != 0) {
        position_pixel_.x +=
            static_cast<float>(dt) * SIM_MOUSE_VELOCITY * static_cast<float>(deltaX);
    }

    if (deltaY != 0) {
        position_pixel_.y +=
            static_cast<float>(dt) * SIM_MOUSE_VELOCITY * static_cast<float>(deltaY);
    }

    movingTimeMs_ += dt;
    const auto maxMovingTimeMs =
        static_cast<unsigned>(std::floor(SIM_CELL_SIDE_LENGTH_PIXEL / SIM_MOUSE_VELOCITY));
    if (movingTimeMs_ > maxMovingTimeMs) {
        game_->getMaze().edge(position_, destination_).travel();
        teleport(destination_);
        movingTimeMs_ = 0;

        destination_ = getNextPosition();
        if (destination_.x() == position_.x() && destination_.y() == position_.y()) {
            stop();
        }
    }
}

}  // namespace Mazemouse
