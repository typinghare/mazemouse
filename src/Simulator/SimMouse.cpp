#include "SimMouse.hpp"

namespace Mazemouse {

SimMouse::SimMouse(SimGame* sim_game) : SimGamePlugin(sim_game) {
    teleport({ SIM_MOUSE_STARTING_POSITION_X, SIM_MOUSE_STARTING_POSITION_Y });
}

void SimMouse::teleport(const sf::Vector2i& position) {
    this->entity_position = position;
    entity_position_pixel = {
        (0.5f + static_cast<float>(position.x)) * SIM_CELL_SIDE_LENGTH_PIXEL,
        (0.5f + static_cast<float>(position.y)) * SIM_CELL_SIDE_LENGTH_PIXEL
    };
}

void SimMouse::update(const unsigned dt) {
    if (!running) {
        nextCycle();
        return;
    }

    const auto deltaX = entity_destination.x - entity_position.x;
    const auto deltaY = entity_destination.y - entity_position.y;

    if (deltaX != 0) {
        entity_position_pixel.x += static_cast<float>(dt) * SIM_MOUSE_VELOCITY *
                                   static_cast<float>(deltaX);
    } else if (deltaY != 0) {
        entity_position_pixel.y += static_cast<float>(dt) * SIM_MOUSE_VELOCITY *
                                   static_cast<float>(deltaY);
    } else {
        return stop();
    }

    movingTimeMs += dt;
    const auto maxMovingTimeMs = static_cast<unsigned>(
        std::floor(SIM_CELL_SIDE_LENGTH_PIXEL / SIM_MOUSE_VELOCITY));
    if (movingTimeMs > maxMovingTimeMs) {
        const auto direction =
            getDirectionByVector(entity_destination - entity_position);
        game->sim_maze.edge(entity_position, direction).num_travelled++;
        teleport(entity_destination);
        movingTimeMs = 0;

        stop();
    }
}

bool SimMouse::checkWall(const Direction dir) {
    const auto abs_dir = getAbsoluteDirection(dir);

    return !game->sim_maze.isEdgeOpen(position, abs_dir);
}

void SimMouse::moveForward(const int length) {
    FloodFillMouse::moveForward(length);

    // Let the destination be the updated position
    entity_destination = position;
    start();
}

void SimMouse::turn(const STEERING steering) {}

void SimMouse::nextCycle() { FloodFillMouse::nextCycle(); }

void SimMouse::draw(
    sf::RenderTarget& target, const sf::RenderStates states) const {
    SimGamePlugin::draw(target, states);
}

void SimMouse::renderOnTexture(sf::RenderTexture& render_texture) {
    static const auto MOUSE_COLOR = sf::Color::Black;

    renderEdges(render_texture);

    auto circle = sf::CircleShape(SIM_MOUSE_RADIUS, 3);
    circle.setFillColor(MOUSE_COLOR);
    circle.setOrigin(SIM_MOUSE_RADIUS, SIM_MOUSE_RADIUS);
    circle.setPosition(entity_position_pixel.x, entity_position_pixel.y);
    circle.rotate(static_cast<float>(to_int(orientation)) * 90);

    render_texture.draw(circle);
}

void SimMouse::renderEdges(sf::RenderTexture& render_texture) const {
    static const auto COLOR = sf::Color::White;

    const auto maze = game->sim_maze;
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
