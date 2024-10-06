#ifndef SIMMOUSE_HPP
#define SIMMOUSE_HPP
#include "SimGame.hpp"

namespace Mazemouse {

constexpr auto SIM_MOUSE_RADIUS = 10.f;
constexpr auto SIM_MOUSE_VELOCITY = 0.4f;

constexpr auto SIM_MOUSE_STARTING_POSITION_X = 0;
constexpr auto SIM_MOUSE_STARTING_POSITION_Y = SIM_MAZE_SIDE_LENGTH - 1;

struct SimMouse final : SimGamePlugin {
    bool running{ false };
    unsigned movingTimeMs{ 0 };
    sf::Vector2i position{ 0, 0 };
    sf::Vector2i destination{ 0, 0 };
    sf::Vector2f position_pixel{ 0, 0 };

    explicit SimMouse(SimGame* sim_game);

    ~SimMouse() override = default;

    void start() { running = true; }

    void stop() { running = false; }

    void teleport(const sf::Vector2i& position);

    [[nodiscard]] sf::Vector2i getNextPosition() const;

    void draw(sf::RenderTexture& maze_texture) override;

    void update(unsigned dt) override;
};

}  // namespace Mazemouse

#endif
