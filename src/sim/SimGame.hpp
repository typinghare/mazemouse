#ifndef SIMGAME_HPP
#define SIMGAME_HPP

#include "SimMaze.hpp"

namespace Mazemouse {

constexpr auto SIM_WINDOW_CAPTION = "Micromouse Simulator";
constexpr auto SIM_WINDOW_MARGIN_PIXEL = 36;

constexpr auto SIM_MAZE_SIDE_LENGTH_PIXEL = SIM_MAZE_SIDE_LENGTH * SIM_CELL_SIDE_LENGTH_PIXEL;
constexpr auto SIM_WINDOW_SIDE_LENGTH_PIXEL = SIM_MAZE_SIDE_LENGTH_PIXEL + 2 * SIM_WINDOW_MARGIN_PIXEL;

constexpr auto SIM_GAME_FPS = 30;
constexpr auto SIM_GAME_VERTICAL_SYNC_ENABLED = true;

class SimGamePlugin;

class SimGame {
    SimMaze maze_{ Size(SIM_MAZE_SIDE_LENGTH, SIM_MAZE_SIDE_LENGTH) };
    sf::RenderTexture maze_render_texture_;
    std::vector<std::shared_ptr<SimGamePlugin>> plugins_;

 public:
    SimGame();

    void run();

    SimMaze& getMaze();

    void addPlugin(const std::shared_ptr<SimGamePlugin>& sim_game_plugin);

 private:
    sf::Sprite renderMaze(sf::RenderTexture& maze_layer_texture);

    static void drawPeripheralWalls(sf::RenderTarget& target);
};

class SimGamePlugin {
 protected:
    SimGame* game_;

 public:
    explicit SimGamePlugin(SimGame* sim_game) : game_(sim_game) {}

    virtual ~SimGamePlugin() = default;

    virtual void draw(sf::RenderTexture& maze_render_texture) = 0;

    virtual void update(unsigned dt) = 0;
};

}  // namespace Mazemouse

#endif
