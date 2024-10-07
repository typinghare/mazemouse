#ifndef SIMMOUSE_HPP
#define SIMMOUSE_HPP
#include "../Mouse/AStarMouse.hpp"
#include "SimGame.hpp"

namespace Mazemouse {

constexpr auto SIM_MOUSE_RADIUS = 10.f;
constexpr auto SIM_MOUSE_VELOCITY = 0.4f;

constexpr auto SIM_MOUSE_STARTING_POSITION_X = 0;
constexpr auto SIM_MOUSE_STARTING_POSITION_Y = SIM_MAZE_SIDE_LENGTH - 1;

struct SimMouse final : SimGamePlugin,
                        AStarMouse<SIM_MAZE_SIDE_LENGTH, FloodFillCell, Edge> {
    bool running{ false };
    unsigned movingTimeMs{ 0 };
    sf::Vector2i entity_position{ 0, 0 };
    sf::Vector2i entity_destination{ 0, 0 };
    sf::Vector2f entity_position_pixel{ 0, 0 };

    explicit SimMouse(SimGame* sim_game);

    ~SimMouse() override = default;

    void start() { running = true; }

    void stop() { running = false; }

    void teleport(const sf::Vector2i& position);

    void update(unsigned dt) override;

    bool checkWall(Direction dir) override;

    void moveForward(int length) override;

    void turn(STEERING steering) override;

    void nextCycle() override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 protected:
    void renderOnTexture(sf::RenderTexture& render_texture) override;

    void renderEdges(sf::RenderTexture& render_texture) const;
};

}  // namespace Mazemouse

#endif
