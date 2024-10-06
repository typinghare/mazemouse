#include "SimMouse.hpp"

#include <iostream>

namespace Mazemouse {

SimMouse::SimMouse(SimGame* sim_game) : SimGamePlugin(sim_game) {
    teleport({ SIM_MOUSE_STARTING_POSITION_X, SIM_MOUSE_STARTING_POSITION_Y });
    destination = getNextPosition();
}

void SimMouse::teleport(const sf::Vector2i& position) {
    this->position = position;
    position_pixel = {
        (0.5f + static_cast<float>(position.x)) * SIM_CELL_SIDE_LENGTH_PIXEL,
        (0.5f + static_cast<float>(position.y)) * SIM_CELL_SIDE_LENGTH_PIXEL
    };
}

sf::Vector2i SimMouse::getNextPosition() const {
    const auto& maze = game->maze;
    const auto x = position.x;
    const auto y = position.y;

    std::cout << "(" << x << ", " << y << ")" << std::endl;
    if (x == 0 && maze.isEdgeOpen(position, Direction::UP)) {
        return { x, y - 1 };
    }

    if (maze.isEdgeOpen(position, Direction::RIGHT) && y == 0) {
        return { x + 1, y };
    }

    if (maze.isEdgeOpen(position, Direction::DOWN) &&
        x == SIM_MAZE_SIDE_LENGTH - 1) {
        return { x, y + 1 };
    }

    if (maze.isEdgeOpen(position, Direction::LEFT) &&
        y == SIM_MAZE_SIDE_LENGTH - 1) {
        return { x - 1, y };
    }

    return position;
}

void SimMouse::draw(sf::RenderTexture& maze_texture) {
    static const auto COLOR = sf::Color::Black;

    auto circle = sf::CircleShape(SIM_MOUSE_RADIUS);
    circle.setFillColor(COLOR);
    circle.setOrigin(SIM_MOUSE_RADIUS, SIM_MOUSE_RADIUS);
    circle.setPosition(position_pixel.x, position_pixel.y);

    maze_texture.draw(circle);
}

void SimMouse::update(const unsigned dt) {
    if (!running) {
        return;
    }

    const auto deltaX = destination.x - position.x;
    const auto deltaY = destination.y - position.y;

    if (deltaX != 0) {
        position_pixel.x += static_cast<float>(dt) * SIM_MOUSE_VELOCITY *
                            static_cast<float>(deltaX);
    }

    if (deltaY != 0) {
        position_pixel.y += static_cast<float>(dt) * SIM_MOUSE_VELOCITY *
                            static_cast<float>(deltaY);
    }

    movingTimeMs += dt;
    const auto maxMovingTimeMs = static_cast<unsigned>(
        std::floor(SIM_CELL_SIDE_LENGTH_PIXEL / SIM_MOUSE_VELOCITY));
    if (movingTimeMs > maxMovingTimeMs) {
        // game->maze.edge(position, Direction::UP).num_travelled++;
        teleport(destination);
        movingTimeMs = 0;

        destination = getNextPosition();
        if (destination.x == position.x && destination.y == position.y) {
            stop();
        }
    }
}

}  // namespace Mazemouse
