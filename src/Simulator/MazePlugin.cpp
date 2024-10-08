#include "MazePlugin.hpp"

namespace MazemouseSimulator {

sf::Vector2i MazePlugin::getWindowSize() {
    static auto SIDE_LENGTH_PIXEL =
        MAZE_MARGIN_PIXEL * 2 + REAL_MAZE_SIDE_LENGTH * CELL_SIDE_LENGTH_PIXEL;
    return { SIDE_LENGTH_PIXEL, SIDE_LENGTH_PIXEL };
}

void PeripheralWallMazePlugin::renderOnTexture(
    sf::RenderTexture& render_texture) {
    static const auto COLOR = sf::Color(13, 19, 33);
    static constexpr auto WALL_THICKNESS_HALF_PIXEL = WALL_THICKNESS_PIXEL / 2;
    static constexpr auto MAZE_SIDE_LENGTH_PIXEL =
        REAL_MAZE_SIDE_LENGTH * CELL_SIDE_LENGTH_PIXEL;

    sf::RectangleShape rectangle_vertical(
        { WALL_THICKNESS_PIXEL,
          MAZE_SIDE_LENGTH_PIXEL + WALL_THICKNESS_PIXEL });
    rectangle_vertical.setFillColor(COLOR);

    // Left
    rectangle_vertical.setPosition(
        MAZE_MARGIN_PIXEL - WALL_THICKNESS_HALF_PIXEL,
        MAZE_MARGIN_PIXEL - WALL_THICKNESS_HALF_PIXEL);
    render_texture.draw(rectangle_vertical);

    // Right
    rectangle_vertical.setPosition(
        MAZE_MARGIN_PIXEL + MAZE_SIDE_LENGTH_PIXEL - WALL_THICKNESS_HALF_PIXEL,
        MAZE_MARGIN_PIXEL - WALL_THICKNESS_HALF_PIXEL);
    render_texture.draw(rectangle_vertical);

    sf::RectangleShape rectangle_horizontal(
        { MAZE_SIDE_LENGTH_PIXEL + WALL_THICKNESS_PIXEL,
          WALL_THICKNESS_PIXEL });
    rectangle_horizontal.setFillColor(COLOR);

    // Top
    rectangle_horizontal.setPosition(
        MAZE_MARGIN_PIXEL - WALL_THICKNESS_HALF_PIXEL,
        MAZE_MARGIN_PIXEL - WALL_THICKNESS_HALF_PIXEL);
    render_texture.draw(rectangle_horizontal);

    // Down
    rectangle_horizontal.setPosition(
        MAZE_MARGIN_PIXEL - WALL_THICKNESS_HALF_PIXEL,
        MAZE_MARGIN_PIXEL + MAZE_SIDE_LENGTH_PIXEL - WALL_THICKNESS_HALF_PIXEL);
    render_texture.draw(rectangle_horizontal);
}

void FloorMazePlugin::renderOnTexture(sf::RenderTexture& render_texture) {
    static const std::vector colors{ sf::Color(255, 221, 210),
                                     sf::Color(131, 197, 190) };
    static const sf::Vector2f cellSize{ CELL_SIDE_LENGTH_PIXEL,
                                        CELL_SIDE_LENGTH_PIXEL };

    for (unsigned col = 0; col < REAL_MAZE_SIDE_LENGTH; ++col) {
        for (unsigned row = 0; row < REAL_MAZE_SIDE_LENGTH; ++row) {
            sf::RectangleShape square(cellSize);
            square.setPosition(
                static_cast<float>(col) * CELL_SIDE_LENGTH_PIXEL,
                static_cast<float>(row) * CELL_SIDE_LENGTH_PIXEL);
            square.setFillColor(colors[(col + row) % colors.size()]);
            render_texture.draw(square);
        }
    }
}

void WallMazePlugin::renderOnTexture(sf::RenderTexture& render_texture) {
    static const auto COLOR = sf::Color::Black;
    static constexpr auto WALL_THICKNESS_HALF_PIXEL = WALL_THICKNESS_PIXEL / 2;

    const auto& maze = game_->getRealMaze();
    sf::RectangleShape rectangle_vertical(
        { WALL_THICKNESS_PIXEL, CELL_SIDE_LENGTH_PIXEL });
    rectangle_vertical.setFillColor(COLOR);
    sf::RectangleShape rectangle_horizontal(
        { CELL_SIDE_LENGTH_PIXEL, WALL_THICKNESS_PIXEL });
    rectangle_horizontal.setFillColor(COLOR);

    // Vertical walls
    for (int col = 0; col < REAL_MAZE_SIDE_LENGTH - 1; ++col) {
        for (int row = 0; row < REAL_MAZE_SIDE_LENGTH; ++row) {
            if (maze.edge({ col, row }, Dir4::Right).hasWall) {
                rectangle_vertical.setPosition(
                    { static_cast<float>(col + 1) * CELL_SIDE_LENGTH_PIXEL -
                          WALL_THICKNESS_HALF_PIXEL,
                      static_cast<float>(row) * CELL_SIDE_LENGTH_PIXEL });
                render_texture.draw(rectangle_vertical);
            }
        }
    }

    // Horizontal walls
    for (int col = 0; col < REAL_MAZE_SIDE_LENGTH; ++col) {
        for (int row = 0; row < REAL_MAZE_SIDE_LENGTH - 1; ++row) {
            if (maze.edge({ col, row }, Dir4::Down).hasWall) {
                rectangle_horizontal.setPosition({
                    static_cast<float>(col) * CELL_SIDE_LENGTH_PIXEL,
                    static_cast<float>(row + 1) * CELL_SIDE_LENGTH_PIXEL -
                        WALL_THICKNESS_HALF_PIXEL,
                });
                render_texture.draw(rectangle_horizontal);
            }
        }
    }
}

void WallMazePlugin::curvePaths(const int seed) const {
    const auto& maze = game_->getRealMaze();

    for (int row = 0; row < REAL_MAZE_SIDE_LENGTH - 1; ++row) {
        for (int col = 0; col < REAL_MAZE_SIDE_LENGTH; ++col) {
            maze.edge({ col, row }, Dir4::Down).hasWall = false;
        }
    }

    for (int row = 0; row < REAL_MAZE_SIDE_LENGTH; ++row) {
        for (int col = 0; col < REAL_MAZE_SIDE_LENGTH - 1; ++col) {
            maze.edge({ col, row }, Dir4::Right).hasWall = false;
        }
    }
}
MouseMazePlugin::MouseMazePlugin(Game* game) :
    MazePlugin(game, MAZE_MARGIN_PIXEL) {
    // Setup entity
    entity_orientation_ = orientation;
    entity_position_ = position;
    entity_destination_ = position;
    teleport(position);

    // Start the mouse
    state = MouseState::Exploring;
}

bool MouseMazePlugin::hardwareCheckWall(const Dir4 dir) {
    return !game_->getRealMaze().isOpen(position, getAbsoluteDir(dir));
}

void MouseMazePlugin::hardwareMoveForward(const int length) {
    entity_destination_ = position + get_vector(entity_orientation_) * length;
    running_ = true;
}

void MouseMazePlugin::hardwareTurn(Dir4 relative_dir) {
    entity_orientation_ = orientation;
}

void MouseMazePlugin::nextCycle() { SemiFinishedMouse::nextCycle(); }

void MouseMazePlugin::moveForward(const int length) {
    FloodFillMouse::moveForward(length);
}

void MouseMazePlugin::renderOnTexture(sf::RenderTexture& render_texture) {
    renderEdges(render_texture);
    renderMouse(render_texture);
}

void MouseMazePlugin::update(const int dt) {
    MazePlugin::update(dt);
    render();

    if (!running_) {
        nextCycle();
    }

    const auto velocity = this->state == MouseState::RushingToFinish
                              ? MOUSE_RUSHING_VELOCITY
                              : MOUSE_EXPLORING_VELOCITY;
    const auto deltaX = entity_destination_.x - entity_position_.x;
    const auto deltaY = entity_destination_.y - entity_position_.y;

    if (deltaX != 0) {
        entity_position_pixel_.x +=
            static_cast<float>(dt) * velocity * static_cast<float>(deltaX);
    } else if (deltaY != 0) {
        entity_position_pixel_.y +=
            static_cast<float>(dt) * velocity * static_cast<float>(deltaY);
    } else {
        running_ = false;
        return;
    }

    moving_time_ms_ += dt;
    const auto maxMovingTimeMs =
        static_cast<unsigned>(std::floor(CELL_SIDE_LENGTH_PIXEL / velocity));
    if (moving_time_ms_ > maxMovingTimeMs) {
        const auto dirVector = entity_destination_ - entity_position_;
        const auto dir = get_dir(dirVector);
        game_->getRealMaze().edge(entity_position_, dir).num_traveled++;
        teleport(entity_destination_);
        moving_time_ms_ = 0;

        // Stop the mouse
        running_ = false;
    }
}

void MouseMazePlugin::renderEdges(sf::RenderTexture& render_texture) const {
    const auto maze = game_->getRealMaze();
    auto rectangle_vertical = sf::RectangleShape({ 2, CELL_SIDE_LENGTH_PIXEL });
    auto rectangle_horizontal =
        sf::RectangleShape({ CELL_SIDE_LENGTH_PIXEL, 2 });

    // Vertical edges
    for (int col = 0; col < REAL_MAZE_SIDE_LENGTH; ++col) {
        for (int row = 0; row < REAL_MAZE_SIDE_LENGTH - 1; ++row) {
            const auto num_traveled =
                maze.edge({ col, row }, Dir4::Down).num_traveled;

            if (num_traveled > 0) {
                rectangle_vertical.setPosition(
                    { static_cast<float>(col + 0.5) * CELL_SIDE_LENGTH_PIXEL,
                      static_cast<float>(row + 0.5) * CELL_SIDE_LENGTH_PIXEL });
                rectangle_vertical.setFillColor(sf::Color(
                    255, 255, 255, std::min(255, 205 + num_traveled * 10)));
                render_texture.draw(rectangle_vertical);
            }
        }
    }

    // Horizontal edges
    for (int col = 0; col < REAL_MAZE_SIDE_LENGTH - 1; ++col) {
        for (int row = 0; row < REAL_MAZE_SIDE_LENGTH; ++row) {
            const auto num_traveled =
                maze.edge({ col, row }, Dir4::Right).num_traveled;
            if (num_traveled > 0) {
                rectangle_horizontal.setPosition(
                    { static_cast<float>(col + 0.5) * CELL_SIDE_LENGTH_PIXEL,
                      static_cast<float>(row + 0.5) * CELL_SIDE_LENGTH_PIXEL });
                rectangle_horizontal.setFillColor(sf::Color(
                    255, 255, 255, std::min(255, 205 + num_traveled * 10)));
                render_texture.draw(rectangle_horizontal);
            }
        }
    }
}

void MouseMazePlugin::renderMouse(sf::RenderTexture& render_texture) const {
    static const auto MOUSE_COLOR = sf::Color::Black;

    auto circle = sf::CircleShape(MOUSE_RADIUS, 3);
    circle.setFillColor(MOUSE_COLOR);
    circle.setOrigin(MOUSE_RADIUS, MOUSE_RADIUS);
    circle.setPosition(entity_position_pixel_.x, entity_position_pixel_.y);
    circle.rotate(static_cast<float>(orientation) * 90);

    render_texture.draw(circle);
}

void MouseMazePlugin::teleport(const sf::Vector2i& position) {
    this->entity_position_ = position;
    entity_position_pixel_ = {
        (0.5f + static_cast<float>(position.x)) * CELL_SIDE_LENGTH_PIXEL,
        (0.5f + static_cast<float>(position.y)) * CELL_SIDE_LENGTH_PIXEL
    };
}

}  // namespace MazemouseSimulator
