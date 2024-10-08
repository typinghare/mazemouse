#ifndef MAZE_PLUGIN_HPP
#define MAZE_PLUGIN_HPP

#include "../Maze/Maze.hpp"
#include "../Mouse/SemiFinishedMouse.hpp"
#include "Game.hpp"

using namespace Mazemouse;

namespace MazemouseSimulator {

constexpr auto MAZE_MARGIN_PIXEL = 36;
constexpr auto CELL_SIDE_LENGTH_PIXEL = 36;
constexpr auto WALL_THICKNESS_PIXEL = 4;
constexpr auto MOUSE_RADIUS = CELL_SIDE_LENGTH_PIXEL / 3;
constexpr auto MOUSE_EXPLORING_VELOCITY = 0.3f;
constexpr auto MOUSE_RUSHING_VELOCITY = 0.8f;

const auto PERIPHERAL_WALL_MAZE_PLUGIN_NAME =
    "PERIPHERAL_WALL_MAZE_PLUGIN_NAME";
const auto FLOOR_MAZE_PLUGIN_NAME = "FLOOR_MAZE_PLUGIN_NAME";
const auto WALL_MAZE_PLUGIN_NAME = "WALL_MAZE_PLUGIN_NAME";
const auto MOUSE_MAZE_PLUGIN_NAME = "MOUSE_MAZE_PLUGIN_NAME";

class Game;

class MazePlugin : public GamePlugin {
 public:
    explicit MazePlugin(Game* game, const int marginPixel = MAZE_MARGIN_PIXEL) :
        GamePlugin(game), marginPixel_(marginPixel) {
        const auto marginPixelFloat = static_cast<float>(marginPixel);
        getSprite().setPosition({ marginPixelFloat, marginPixelFloat });
    }

    static sf::Vector2i getWindowSize();

    void update(int dt) override{};

    ~MazePlugin() override = default;

 protected:
    int marginPixel_;
};

class PeripheralWallMazePlugin final : public MazePlugin {
 public:
    explicit PeripheralWallMazePlugin(Game* game) : MazePlugin(game, 0) {
        render();
    }

    std::string getName() override { return PERIPHERAL_WALL_MAZE_PLUGIN_NAME; }

 protected:
    void renderOnTexture(sf::RenderTexture& render_texture) override;

 public:
    void update(int dt) override {}

    ~PeripheralWallMazePlugin() override = default;
};

class FloorMazePlugin final : public MazePlugin {
 public:
    explicit FloorMazePlugin(Game* game) : MazePlugin(game, MAZE_MARGIN_PIXEL) {
        render();
    }

    std::string getName() override { return FLOOR_MAZE_PLUGIN_NAME; }

 protected:
    void renderOnTexture(sf::RenderTexture& render_texture) override;
};

class WallMazePlugin final : public MazePlugin {
 public:
    explicit WallMazePlugin(Game* game) : MazePlugin(game, MAZE_MARGIN_PIXEL) {
        curvePaths(900);
        render();
    }

    std::string getName() override { return WALL_MAZE_PLUGIN_NAME; }

 protected:
    void renderOnTexture(sf::RenderTexture& render_texture) override;

 public:
    void curvePaths(int seed) const;
};

class MouseMazePlugin final : public MazePlugin,
                              public SemiFinishedMouse<REAL_MAZE_SIDE_LENGTH> {
 public:
    explicit MouseMazePlugin(Game* game);

    std::string getName() override { return MOUSE_MAZE_PLUGIN_NAME; }

    bool hardwareCheckWall(Dir4 dir) override;

    void hardwareMoveForward(int length) override;

    void hardwareTurn(Dir4 relative_dir) override;

    void nextCycle() override;

    void moveForward(int length) override;

 protected:
    void renderOnTexture(sf::RenderTexture& render_texture) override;

    void update(int dt) override;

 private:
    bool running_{ false };
    unsigned moving_time_ms_{ 0 };
    Dir4 entity_orientation_{ Dir4::Up };
    sf::Vector2i entity_position_{ 0, 0 };
    sf::Vector2i entity_destination_{ 0, 0 };
    sf::Vector2f entity_position_pixel_{ 0, 0 };

    void renderEdges(sf::RenderTexture& render_texture) const;

    void renderMouse(sf::RenderTexture& render_texture) const;

    void teleport(const sf::Vector2i& position);
};

}  // namespace MazemouseSimulator

#endif
