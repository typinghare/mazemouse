#include "MazePlugin.hpp"
#include <random>

namespace MazemouseSimulator {

Vector2 MazePlugin::getWindowSize() {
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

    renderGoalArea(render_texture);
}

void FloorMazePlugin::renderGoalArea(sf::RenderTexture& render_texture) {
    static const auto COLOR = sf::Color(239, 71, 111, 125);

    constexpr auto G = CELL_SIDE_LENGTH_PIXEL * (REAL_MAZE_SIDE_LENGTH / 2 - 1);
    sf::RectangleShape goalArea(
        { CELL_SIDE_LENGTH_PIXEL * 2, CELL_SIDE_LENGTH_PIXEL * 2 });
    goalArea.setPosition(G, G);
    goalArea.setFillColor(COLOR);
    render_texture.draw(goalArea);
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

void WallMazePlugin::carvePaths(const int seed) const {
    const auto& maze = game_->getRealMaze();
    constexpr auto S = REAL_MAZE_SIDE_LENGTH;

    std::mt19937 rng(seed);
    std::vector visited(S * S, false);
    std::stack<Vector2> cell_stack;
    Vector2 current(0, 15);
    cell_stack.push(current);
    visited[current.y * S + current.x] = true;

    constexpr int halfSide = S / 2;
    const std::vector centerCells = {
        Vector2(halfSide - 1, halfSide - 1),
        Vector2(halfSide - 1, halfSide),
        Vector2(halfSide, halfSide),
        Vector2(halfSide, halfSide - 1),
    };
    bool reached_center = false;

    while (!cell_stack.empty()) {
        current = cell_stack.top();

        // Check if we've reached any of the center cells
        for (const auto& centerCell : centerCells) {
            if (current == centerCell) {
                reached_center = true;
                break;
            }
        }

        // Get possible directions
        std::vector<Dir4> possible_dirs;
        for (int i = 0; i < 4; i++) {
            auto dir = static_cast<Dir4>(i);
            const auto [x, y] = current + get_vector(static_cast<Dir4>(i));

            // Check if the next cell is within bounds
            if (x >= 0 && x < S && y >= 0 && y < S) {
                if (!visited[y * S + x]) {
                    possible_dirs.push_back(dir);
                }
            }
        }

        if (!possible_dirs.empty()) {
            std::uniform_int_distribution dist(
                0, static_cast<int>(possible_dirs.size()) - 1);
            const Dir4 chosenDir = possible_dirs[dist(rng)];
            maze.edge(current, chosenDir).hasWall = false;

            Vector2 next = current + get_vector(chosenDir);
            visited[next.y * S + next.x] = true;
            cell_stack.push(next);
        } else {
            cell_stack.pop();
        }
    }

    if (!reached_center) {
        std::vector visited2(S * S, false);
        std::function<bool(Vector2)> findPath = [&](const Vector2 pos) -> bool {
            visited2[pos.y * S + pos.x] = true;

            for (const auto& centerCell : centerCells) {
                if (pos == centerCell)
                    return true;
            }

            // Try all directions
            for (int i = 0; i < 4; i++) {
                const auto dir = static_cast<Dir4>(i);
                const Vector2 next = pos + get_vector(dir);

                if (next.x >= 0 && next.x < S && next.y >= 0 && next.y < S &&
                    !visited2[next.y * S + next.x]) {
                    const Edge& currentEdge = maze.edge(pos, dir);
                    currentEdge.hasWall = false;

                    if (findPath(next))
                        return true;

                    currentEdge.hasWall = true;
                }
            }
            return false;
        };

        findPath(Vector2(0, 15));
    }

    // Connect center cells to each other
    auto dir = Dir4::Down;
    for (auto centerCell : centerCells) {
        maze.edge(centerCell, dir).hasWall = false;
        dir = dir + Dir4::Left;
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
    entity_destination_ = position + length * get_vector(entity_orientation_);
    running_ = true;
}

void MouseMazePlugin::hardwareTurn(Dir4 relative_dir) {
    entity_orientation_ = orientation;
}

void MouseMazePlugin::nextExploringCycle() {
    SemiFinishedMouse::nextExploringCycle();
}

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
        if (state == MouseState::Exploring ||
            state == MouseState::ReturningToStart) {
            nextExploringCycle();
        } else if (state == MouseState::RushingToFinish) {
            nextRushingCycle();
        } else if (state == MouseState::Stopped) {
            running_ = false;
            return;
        }
    }

    const auto velocity = this->state == MouseState::RushingToFinish
                              ? MOUSE_RUSHING_VELOCITY
                              : MOUSE_EXPLORING_VELOCITY;
    const auto deltaX = entity_destination_.x - entity_position_.x;
    const auto deltaY = entity_destination_.y - entity_position_.y;

    int step;
    if (deltaX != 0) {
        const auto velocitySign = static_cast<float>(deltaX > 0 ? 1 : -1);
        entity_position_pixel_.x +=
            static_cast<float>(dt) * velocity * velocitySign;
        step = std::abs(deltaX);
    } else if (deltaY != 0) {
        const auto velocitySign = static_cast<float>(deltaY > 0 ? 1 : -1);
        entity_position_pixel_.y +=
            static_cast<float>(dt) * velocity * velocitySign;
        step = std::abs(deltaY);
    } else {
        running_ = false;
        return;
    }

    moving_time_ms_ += dt;
    const auto maxMovingTimeMs =
        static_cast<unsigned>(std::floor(CELL_SIDE_LENGTH_PIXEL / velocity)) *
        step;
    if (moving_time_ms_ > maxMovingTimeMs) {
        auto dirVector = entity_destination_ - entity_position_;
        if (dirVector.x != 0) {
            dirVector.x = dirVector.x > 0 ? 1 : -1;
        }
        if (dirVector.y != 0) {
            dirVector.y = dirVector.y > 0 ? 1 : -1;
        }

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
                    255, 255, 255, std::min(255, 215 + num_traveled * 10)));
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
                    255, 255, 255, std::min(255, 215 + num_traveled * 10)));
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

void MouseMazePlugin::teleport(const Vector2& position) {
    this->entity_position_ = position;
    entity_position_pixel_ = {
        (0.5f + static_cast<float>(position.x)) * CELL_SIDE_LENGTH_PIXEL,
        (0.5f + static_cast<float>(position.y)) * CELL_SIDE_LENGTH_PIXEL
    };
}

void StateDisplayMazePlugin::renderOnTexture(
    sf::RenderTexture& render_texture) {
    const auto state =
        this->game_->getPlugin<MouseMazePlugin>(MOUSE_MAZE_PLUGIN_NAME)
            ->getState();

    auto text = sf::Text();
    text.setFont(font);
    text.setString(getStringByState(state));
    text.setCharacterSize(30);
    text.setFillColor(getColorByState(state));

    render_texture.draw(text);
}

std::string StateDisplayMazePlugin::getStringByState(const MouseState& state) {
    switch (state) {
        case MouseState::Stopped:
            return "STOPPED";
        case MouseState::Exploring:
            return "EXPLORING";
        case MouseState::ReturningToStart:
            return "RETURNING TO START";
        case MouseState::RushingToFinish:
            return "RUSHING TO FINISH";
    }
}

sf::Color StateDisplayMazePlugin::getColorByState(const MouseState& state) {
    static const auto COLOR_STOPPED = sf::Color(239, 71, 111);
    static const auto COLOR_EXPLORING = sf::Color(255, 209, 102);
    static const auto COLOR_RETURNING_TO_START = sf::Color(17, 138, 178);
    static const auto COLOR_RUSHING_TO_FINISH = sf::Color(6, 214, 160);

    switch (state) {
        case MouseState::Stopped:
            return COLOR_STOPPED;
        case MouseState::Exploring:
            return COLOR_EXPLORING;
        case MouseState::ReturningToStart:
            return COLOR_RETURNING_TO_START;
        case MouseState::RushingToFinish:
            return COLOR_RUSHING_TO_FINISH;
    }
}

}  // namespace MazemouseSimulator
