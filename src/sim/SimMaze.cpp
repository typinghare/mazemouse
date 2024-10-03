#include "SimMaze.hpp"

namespace Mazemouse {

void SimEdge::travel() { num_travelled_++; }

int SimEdge::numTravelled() const { return num_travelled_; }

SimMaze::SimMaze(const Size size) : Maze(size) { generateWalls(); }

void SimMaze::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    drawCells(target);
    drawFinishingCells(target);
    drawWalls(target);
    drawEdges(target);
}

void SimMaze::generateWalls() const {
    for (unsigned row = 0; row < SIM_MAZE_SIDE_LENGTH - 1; ++row) {
        edge({ 0, row }, { 0, row + 1 }).setExisted();
        edge({ SIM_MAZE_SIDE_LENGTH - 1, row }, { SIM_MAZE_SIDE_LENGTH - 1, row + 1 }).setExisted();
    }

    for (unsigned col = 0; col < SIM_MAZE_SIDE_LENGTH - 1; ++col) {
        edge({ col, 0 }, { col + 1, 0 }).setExisted();
        edge({ col, SIM_MAZE_SIDE_LENGTH - 1 }, { col + 1, SIM_MAZE_SIDE_LENGTH - 1 }).setExisted();
    }
}

void SimMaze::drawCells(sf::RenderTarget& target) const {
    static const std::vector colors{ sf::Color(255, 221, 210), sf::Color(131, 197, 190) };
    static const sf::Vector2f cellSize{ SIM_CELL_SIDE_LENGTH_PIXEL, SIM_CELL_SIDE_LENGTH_PIXEL };

    for (unsigned col = 0; col < size_.width(); ++col) {
        for (unsigned row = 0; row < size_.height(); ++row) {
            sf::RectangleShape square(cellSize);
            square.setPosition(
                static_cast<float>(col) * SIM_CELL_SIDE_LENGTH_PIXEL,
                static_cast<float>(row) * SIM_CELL_SIDE_LENGTH_PIXEL);
            square.setFillColor(colors[(col + row) % colors.size()]);
            target.draw(square);
        }
    }
}

void SimMaze::drawFinishingCells(sf::RenderTarget& target) {
    static const auto COLOR = sf::Color(255, 179, 198, 128);
    static const sf::Vector2f cellSize{ SIM_CELL_SIDE_LENGTH_PIXEL, SIM_CELL_SIDE_LENGTH_PIXEL };

    sf::RectangleShape square(cellSize);
    square.setFillColor(COLOR);
    for (unsigned col = 7; col < 9; ++col) {
        for (unsigned row = 7; row < 9; ++row) {
            square.setPosition(
                static_cast<float>(col) * SIM_CELL_SIDE_LENGTH_PIXEL,
                static_cast<float>(row) * SIM_CELL_SIDE_LENGTH_PIXEL);
            target.draw(square);
        }
    }
}

void SimMaze::drawWalls(sf::RenderTarget& target) const {
    static const auto COLOR = sf::Color::Black;
    static constexpr auto SIM_WALL_THICKNESS_HALF_PIXEL = SIM_WALL_THICKNESS_PIXEL / 2;

    sf::RectangleShape rectangle_vertical({ SIM_WALL_THICKNESS_PIXEL, SIM_CELL_SIDE_LENGTH_PIXEL });
    rectangle_vertical.setFillColor(COLOR);
    sf::RectangleShape rectangle_horizontal(
        { SIM_CELL_SIDE_LENGTH_PIXEL, SIM_WALL_THICKNESS_PIXEL });
    rectangle_horizontal.setFillColor(COLOR);

    // Vertical walls
    for (int col = 0; col < SIM_MAZE_SIDE_LENGTH - 1; ++col) {
        for (int row = 0; row < SIM_MAZE_SIDE_LENGTH - 1; ++row) {
            if (!edge({ col, row }, { col + 1, row }).existed()) {
                rectangle_vertical.setPosition(
                    { static_cast<float>(col + 1) * SIM_CELL_SIDE_LENGTH_PIXEL -
                          SIM_WALL_THICKNESS_HALF_PIXEL,
                      static_cast<float>(row) * SIM_CELL_SIDE_LENGTH_PIXEL });
                target.draw(rectangle_vertical);
            }
        }
    }

    // Horizontal walls
    for (int col = 0; col < SIM_MAZE_SIDE_LENGTH - 1; ++col) {
        for (int row = 0; row < SIM_MAZE_SIDE_LENGTH - 1; ++row) {
            if (!edge({ col, row }, { col, row + 1 }).existed()) {
                rectangle_horizontal.setPosition({
                    static_cast<float>(col) * SIM_CELL_SIDE_LENGTH_PIXEL,
                    static_cast<float>(row + 1) * SIM_CELL_SIDE_LENGTH_PIXEL -
                        SIM_WALL_THICKNESS_HALF_PIXEL,
                });
                target.draw(rectangle_horizontal);
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
            const auto num_traveled = edge({ col, row }, { col, row + 1 }).numTravelled();
            if (num_traveled > 0) {
                rectangle_vertical.setPosition(
                    { static_cast<float>(col + 0.5) * SIM_CELL_SIDE_LENGTH_PIXEL,
                      static_cast<float>(row + 0.5) * SIM_CELL_SIDE_LENGTH_PIXEL });
                target.draw(rectangle_vertical);
            }
        }
    }

    // Horizontal edges
    for (int col = 0; col < SIM_MAZE_SIDE_LENGTH - 1; ++col) {
        for (int row = 0; row < SIM_MAZE_SIDE_LENGTH; ++row) {
            const auto num_traveled = edge({ col + 1, row }, { col, row }).numTravelled();
            if (num_traveled > 0) {
                rectangle_horizontal.setPosition(
                    { static_cast<float>(col + 0.5) * SIM_CELL_SIDE_LENGTH_PIXEL,
                      static_cast<float>(row + 0.5) * SIM_CELL_SIDE_LENGTH_PIXEL });
                target.draw(rectangle_horizontal);
            }
        }
    }
}

}  // namespace Mazemouse
