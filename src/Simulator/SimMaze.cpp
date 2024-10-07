#include "SimMaze.hpp"
#include <random>
#include <stack>
#include <vector>

namespace Mazemouse {

void SimMaze::renderOnTexture(sf::RenderTexture& render_texture) {
    renderCellsTexture(render_texture);
    renderWallsTexture(render_texture);
}

SimMaze::SimMaze() {
    generatePaths(SIM_MAZE_RANDOM_SEED);
    render();
}

// ReSharper disable once CppDFAConstantParameter
void SimMaze::generatePaths(const int seed) const {
    std::mt19937 rng(seed);
    std::vector visited(SIM_MAZE_SIDE_LENGTH * SIM_MAZE_SIDE_LENGTH, false);
    std::stack<sf::Vector2i> cellStack;
    sf::Vector2i current(0, 15);
    cellStack.push(current);
    visited[current.y * SIM_MAZE_SIDE_LENGTH + current.x] = true;

    // Define the center cells
    const std::vector centerCells = { sf::Vector2i(7, 7), sf::Vector2i(7, 8),
                                      sf::Vector2i(8, 7), sf::Vector2i(8, 8) };
    bool reachedCenter = false;

    while (!cellStack.empty()) {
        current = cellStack.top();

        // Check if we've reached any of the center cells
        for (const auto& centerCell : centerCells) {
            if (current == centerCell) {
                reachedCenter = true;
                break;
            }
        }

        // Get possible directions
        std::vector<Direction> possibleDirs;
        for (int i = 0; i < 4; i++) {
            auto dir = static_cast<Direction>(i);
            const sf::Vector2i next = current + DirectionVectors[i];

            // Check if the next cell is within bounds
            if (next.x >= 0 && next.x < SIM_MAZE_SIDE_LENGTH && next.y >= 0 &&
                next.y < SIM_MAZE_SIDE_LENGTH) {
                if (!visited[next.y * SIM_MAZE_SIDE_LENGTH + next.x]) {
                    possibleDirs.push_back(dir);
                }
            }
        }

        if (!possibleDirs.empty()) {
            std::uniform_int_distribution<> dist(
                0, static_cast<int>(possibleDirs.size()) - 1);
            Direction chosenDir = possibleDirs[dist(rng)];

            const Edge& currentEdge = edge(current, chosenDir);
            currentEdge.hasWall = false;

            sf::Vector2i next =
                current + DirectionVectors[static_cast<int>(chosenDir)];
            visited[next.y * SIM_MAZE_SIDE_LENGTH + next.x] = true;
            cellStack.push(next);
        } else {
            cellStack.pop();
        }
    }

    if (!reachedCenter) {
        std::vector visited2(
            SIM_MAZE_SIDE_LENGTH * SIM_MAZE_SIDE_LENGTH, false);
        std::function<bool(sf::Vector2i)> findPath =
            [&](const sf::Vector2i pos) -> bool {
            visited2[pos.y * SIM_MAZE_SIDE_LENGTH + pos.x] = true;

            for (const auto& centerCell : centerCells) {
                if (pos == centerCell)
                    return true;
            }

            // Try all directions
            for (int i = 0; i < 4; i++) {
                const auto dir = static_cast<Direction>(i);
                const sf::Vector2i next = pos + DirectionVectors[i];

                if (next.x >= 0 && next.x < SIM_MAZE_SIDE_LENGTH &&
                    next.y >= 0 && next.y < SIM_MAZE_SIDE_LENGTH &&
                    !visited2[next.y * SIM_MAZE_SIDE_LENGTH + next.x]) {

                    const Edge& currentEdge = edge(pos, dir);
                    currentEdge.hasWall = false;

                    if (findPath(next))
                        return true;

                    currentEdge.hasWall = true;
                }
            }
            return false;
        };

        findPath(sf::Vector2i(0, 15));
    }

    // Connect center cells to each other
    for (size_t i = 0; i < centerCells.size(); i++) {
        for (size_t j = i + 1; j < centerCells.size(); j++) {
            const sf::Vector2i diff = centerCells[j] - centerCells[i];
            if (abs(diff.x) + abs(diff.y) == 1) {  // If cells are adjacent
                Direction dir;
                if (diff.x == 1)
                    dir = Direction::RIGHT;
                else if (diff.x == -1)
                    dir = Direction::LEFT;
                else if (diff.y == 1)
                    dir = Direction::DOWN;
                else
                    dir = Direction::UP;

                const Edge& centerEdge = edge(centerCells[i], dir);
                centerEdge.hasWall = false;
            }
        }
    }
}

void SimMaze::renderCellsTexture(sf::RenderTexture& render_texture) {
    static const std::vector colors{ sf::Color(255, 221, 210),
                                     sf::Color(131, 197, 190) };
    static const sf::Vector2f cellSize{ SIM_CELL_SIDE_LENGTH_PIXEL,
                                        SIM_CELL_SIDE_LENGTH_PIXEL };

    for (unsigned col = 0; col < SIM_MAZE_SIDE_LENGTH; ++col) {
        for (unsigned row = 0; row < SIM_MAZE_SIDE_LENGTH; ++row) {
            sf::RectangleShape square(cellSize);
            square.setPosition(
                static_cast<float>(col) * SIM_CELL_SIDE_LENGTH_PIXEL,
                static_cast<float>(row) * SIM_CELL_SIDE_LENGTH_PIXEL);
            square.setFillColor(colors[(col + row) % colors.size()]);
            render_texture.draw(square);
        }
    }
}

void SimMaze::renderWallsTexture(sf::RenderTexture& render_texture) const {
    static const auto COLOR = sf::Color::Black;
    static constexpr auto SIM_WALL_THICKNESS_HALF_PIXEL =
        SIM_WALL_THICKNESS_PIXEL / 2;

    sf::RectangleShape rectangle_vertical(
        { SIM_WALL_THICKNESS_PIXEL, SIM_CELL_SIDE_LENGTH_PIXEL });
    rectangle_vertical.setFillColor(COLOR);
    sf::RectangleShape rectangle_horizontal(
        { SIM_CELL_SIDE_LENGTH_PIXEL, SIM_WALL_THICKNESS_PIXEL });
    rectangle_horizontal.setFillColor(COLOR);

    // Vertical walls
    for (int col = 0; col < SIM_MAZE_SIDE_LENGTH - 1; ++col) {
        for (int row = 0; row < SIM_MAZE_SIDE_LENGTH; ++row) {
            if (!isEdgeOpen({ col, row }, Direction::RIGHT)) {
                rectangle_vertical.setPosition(
                    { static_cast<float>(col + 1) * SIM_CELL_SIDE_LENGTH_PIXEL -
                          SIM_WALL_THICKNESS_HALF_PIXEL,
                      static_cast<float>(row) * SIM_CELL_SIDE_LENGTH_PIXEL });
                render_texture.draw(rectangle_vertical);
            }
        }
    }

    // Horizontal walls
    for (int col = 0; col < SIM_MAZE_SIDE_LENGTH; ++col) {
        for (int row = 0; row < SIM_MAZE_SIDE_LENGTH - 1; ++row) {
            if (!isEdgeOpen({ col, row }, Direction::DOWN)) {
                rectangle_horizontal.setPosition({
                    static_cast<float>(col) * SIM_CELL_SIDE_LENGTH_PIXEL,
                    static_cast<float>(row + 1) * SIM_CELL_SIDE_LENGTH_PIXEL -
                        SIM_WALL_THICKNESS_HALF_PIXEL,
                });
                render_texture.draw(rectangle_horizontal);
            }
        }
    }
}

}  // namespace Mazemouse
