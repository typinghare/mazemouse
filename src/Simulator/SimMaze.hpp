#ifndef SIMMAZE_HPP
#define SIMMAZE_HPP

#include <SFML/Graphics.hpp>
#include "../Maze.hpp"
#include "SimGameObject.hpp"

namespace Mazemouse {

constexpr auto SIM_CELL_SIDE_LENGTH_PIXEL = 36;
constexpr auto SIM_MAZE_SIDE_LENGTH = 16;
constexpr auto SIM_MAZE_SIDE_LENGTH_PIXEL =
    SIM_MAZE_SIDE_LENGTH * SIM_CELL_SIDE_LENGTH_PIXEL;
constexpr auto SIM_WALL_THICKNESS_PIXEL = 4;

constexpr auto SIM_MAZE_RANDOM_SEED = 400;

struct SimCell : Cell {};

struct SimEdge : Edge {
    mutable int num_travelled = 0;
};

struct SimMazeObject : SimGameObject {
    SimMazeObject() :
        SimGameObject{ { SIM_MAZE_SIDE_LENGTH_PIXEL,
                         SIM_MAZE_SIDE_LENGTH_PIXEL } } {}
};

struct SimMaze final : Maze<SIM_MAZE_SIDE_LENGTH, SimCell, SimEdge>,
                       SimMazeObject {
    SimMaze();

 protected:
    void renderOnTexture(sf::RenderTexture& render_texture) override;

 private:
    static void renderCellsTexture(sf::RenderTexture& render_texture);

    void renderWallsTexture(sf::RenderTexture& render_texture) const;

    void generatePaths(int seed) const;
};

}  // namespace Mazemouse

#endif
