#ifndef SIMMAZE_HPP
#define SIMMAZE_HPP

#include <SFML/Graphics.hpp>
#include "../maze/Cell.hpp"
#include "../maze/Edge.hpp"
#include "../maze/Maze.hpp"

namespace Mazemouse {

constexpr auto SIM_CELL_SIDE_LENGTH_PIXEL = 36.f;
constexpr auto SIM_MAZE_SIDE_LENGTH = 16;
constexpr auto SIM_WALL_THICKNESS_PIXEL = 4.f;

class SimCell : public Cell {};

class SimEdge : public Edge {
    int num_travelled_ = 0;

 public:
    void travel();

    [[nodiscard]] int numTravelled() const;
};

class SimMaze final : public Maze<SimCell, SimEdge>, public sf::Drawable {
 public:
    explicit SimMaze(Size size);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
    void generateWalls() const;

    void drawCells(sf::RenderTarget& target) const;

    static void drawFinishingCells(sf::RenderTarget& target);

    void drawWalls(sf::RenderTarget& target) const;

    void drawEdges(sf::RenderTarget& target) const;
};

}  // namespace Mazemouse

#endif
