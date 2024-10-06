#include "SimMouse.hpp"

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

    if (y != 14 && maze.isEdgeOpen(position, Direction::RIGHT)) {
        return { x + 1, y };
    }

    if (maze.isEdgeOpen(position, Direction::UP)) {
        return { x, y - 1 };
    }

    if (maze.isEdgeOpen(position, Direction::LEFT)) {
        return { x - 1, y };
    }

    if (maze.isEdgeOpen(position, Direction::DOWN)) {
        return { x, y + 1 };
    }

    return position;
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
        const auto direction = getDirectionByVector(destination - position);
        game->maze.edge(position, direction).num_travelled++;
        teleport(destination);
        movingTimeMs = 0;

        destination = getNextPosition();
        if (destination.x == position.x && destination.y == position.y) {
            stop();
        }
    }
}

void SimMouse::renderOnTexture(sf::RenderTexture& render_texture) {
    static const auto MOUSE_COLOR = sf::Color::Black;

    renderEdges(render_texture);

    auto circle = sf::CircleShape(SIM_MOUSE_RADIUS);
    circle.setFillColor(MOUSE_COLOR);
    circle.setOrigin(SIM_MOUSE_RADIUS, SIM_MOUSE_RADIUS);
    circle.setPosition(position_pixel.x, position_pixel.y);

    render_texture.draw(circle);
}

void SimMouse::renderEdges(sf::RenderTexture& render_texture) const {
    static const auto COLOR = sf::Color::White;

    const auto maze = game->maze;
    auto rectangle_vertical =
        sf::RectangleShape({ 2, SIM_CELL_SIDE_LENGTH_PIXEL });
    auto rectangle_horizontal =
        sf::RectangleShape({ SIM_CELL_SIDE_LENGTH_PIXEL, 2 });
    rectangle_vertical.setFillColor(COLOR);
    rectangle_horizontal.setFillColor(COLOR);

    // Vertical edges
    for (int col = 0; col < SIM_MAZE_SIDE_LENGTH; ++col) {
        for (int row = 0; row < SIM_MAZE_SIDE_LENGTH - 1; ++row) {
            const auto num_traveled =
                maze.edge({ col, row }, Direction::DOWN).num_travelled;

            if (num_traveled > 0) {
                rectangle_vertical.setPosition(
                    { static_cast<float>(col + 0.5) *
                          SIM_CELL_SIDE_LENGTH_PIXEL,
                      static_cast<float>(row + 0.5) *
                          SIM_CELL_SIDE_LENGTH_PIXEL });
                render_texture.draw(rectangle_vertical);
            }
        }
    }

    // Horizontal edges
    for (int col = 0; col < SIM_MAZE_SIDE_LENGTH - 1; ++col) {
        for (int row = 0; row < SIM_MAZE_SIDE_LENGTH; ++row) {
            const auto num_traveled =
                maze.edge({ col, row }, Direction::RIGHT).num_travelled;
            if (num_traveled > 0) {
                rectangle_horizontal.setPosition(
                    { static_cast<float>(col + 0.5) *
                          SIM_CELL_SIDE_LENGTH_PIXEL,
                      static_cast<float>(row + 0.5) *
                          SIM_CELL_SIDE_LENGTH_PIXEL });
                render_texture.draw(rectangle_horizontal);
            }
        }
    }
}

}  // namespace Mazemouse
