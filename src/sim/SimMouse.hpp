#ifndef SIMMOUSE_HPP
#define SIMMOUSE_HPP

#include <SFML/Graphics.hpp>
#include "../common/Coordinate.hpp"
#include "SimGame.hpp"

namespace Mazemouse {

constexpr auto SIM_MOUSE_RADIUS = 10.f;
constexpr auto SIM_MOUSE_VELOCITY = 0.4f;

constexpr auto SIM_MOUSE_STARTING_POSITION_X = 0;
constexpr auto SIM_MOUSE_STARTING_POSITION_Y = SIM_MAZE_SIDE_LENGTH - 1;

class SimMouse final : public SimGamePlugin {
    bool running_{ false };
    unsigned movingTimeMs_{ 0 };
    Coordinate position_{ 0, 0 };
    Coordinate destination_{ 0, 0 };
    sf::Vector2f position_pixel_{ 0, 0 };

 public:
    explicit SimMouse(SimGame* sim_game);

    ~SimMouse() override = default;

    void start() { running_ = true; }

    void stop() { running_ = false; }

    void teleport(const Coordinate& position);

    [[nodiscard]] Coordinate getNextPosition() const;

    void draw(sf::RenderTexture& maze_render_texture) override;

    void update(unsigned dt) override;
};

}  // namespace Mazemouse

#endif
