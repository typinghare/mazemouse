#ifndef SIMGAME_HPP
#define SIMGAME_HPP

#include "SimMaze.hpp"

namespace Mazemouse {

constexpr auto SIM_WINDOW_CAPTION = "Micromouse Simulator";
constexpr auto SIM_WINDOW_MARGIN_PIXEL = 36;

constexpr auto SIM_WINDOW_SIDE_LENGTH_PIXEL =
    SIM_MAZE_SIDE_LENGTH_PIXEL + 2 * SIM_WINDOW_MARGIN_PIXEL;

constexpr auto SIM_GAME_FPS = 60;
constexpr auto SIM_GAME_VERTICAL_SYNC_ENABLED = true;

struct SimGamePlugin;

struct SimGame {
    SimMaze maze;
    std::vector<std::shared_ptr<SimGamePlugin>> plugins;

    void run() const;

    void addPlugin(const std::shared_ptr<SimGamePlugin>& plugin);

    void drawMaze(sf::RenderTarget& target) const;

    static void drawPeripheralWalls(sf::RenderTarget& target);
};

struct SimGamePlugin : virtual SimMazeObject {
    SimGame* game;

    explicit SimGamePlugin(SimGame* game) : game(game) {}

    ~SimGamePlugin() override = default;

    virtual void update(unsigned dt) = 0;
};

}  // namespace Mazemouse

#endif
