#include "SimMaze.hpp"

namespace Mazemouse {

void SimMaze::renderOnTexture(sf::RenderTexture& render_texture) {
    renderCellsTexture(render_texture);
    renderWallsTexture(render_texture);
}

SimMaze::SimMaze() :
    SimGameObject{ { SIM_MAZE_SIDE_LENGTH_PIXEL,
                     SIM_MAZE_SIDE_LENGTH_PIXEL } } {
    generatePaths();
    render();
}

void SimMaze::generatePaths() const {
    for (int row = 0; row < SIM_MAZE_SIDE_LENGTH - 1; ++row) {
        edge({ 0, row }, Direction::DOWN).hasWall = false;
        edge({ SIM_MAZE_SIDE_LENGTH - 1, row }, Direction::DOWN).hasWall =
            false;
    }

    for (int col = 0; col < SIM_MAZE_SIDE_LENGTH - 1; ++col) {
        edge({ col, 0 }, Direction::RIGHT).hasWall = false;
        edge({ col, SIM_MAZE_SIDE_LENGTH - 1 }, Direction::RIGHT).hasWall =
            false;
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
        for (int row = 0; row < SIM_MAZE_SIDE_LENGTH - 1; ++row) {
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
    for (int col = 0; col < SIM_MAZE_SIDE_LENGTH - 1; ++col) {
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

void SimMaze::drawEdges(sf::RenderTarget& target) const {
    static const auto COLOR = sf::Color::White;

    sf::RectangleShape rectangle_vertical({ 2, SIM_CELL_SIDE_LENGTH_PIXEL });
    rectangle_vertical.setFillColor(COLOR);
    sf::RectangleShape rectangle_horizontal({ SIM_CELL_SIDE_LENGTH_PIXEL, 2 });
    rectangle_horizontal.setFillColor(COLOR);

    // Vertical edges
    for (int col = 0; col < SIM_MAZE_SIDE_LENGTH; ++col) {
        for (int row = 0; row < SIM_MAZE_SIDE_LENGTH - 1; ++row) {
            const auto num_traveled =
                edge({ col, row }, Direction::DOWN).num_travelled;

            if (num_traveled > 0) {
                rectangle_vertical.setPosition(
                    { static_cast<float>(col + 0.5) *
                          SIM_CELL_SIDE_LENGTH_PIXEL,
                      static_cast<float>(row + 0.5) *
                          SIM_CELL_SIDE_LENGTH_PIXEL });
                target.draw(rectangle_vertical);
            }
        }
    }

    // Horizontal edges
    for (int col = 0; col < SIM_MAZE_SIDE_LENGTH - 1; ++col) {
        for (int row = 0; row < SIM_MAZE_SIDE_LENGTH; ++row) {
            const auto num_traveled =
                edge({ col, row }, Direction::RIGHT).num_travelled;
            if (num_traveled > 0) {
                rectangle_horizontal.setPosition(
                    { static_cast<float>(col + 0.5) *
                          SIM_CELL_SIDE_LENGTH_PIXEL,
                      static_cast<float>(row + 0.5) *
                          SIM_CELL_SIDE_LENGTH_PIXEL });
                target.draw(rectangle_horizontal);
            }
        }
    }
}

}  // namespace Mazemouse
