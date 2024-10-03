#include "sim/SimGame.hpp"
#include "sim/SimMouse.hpp"

int main() {
    auto sim_game = Mazemouse::SimGame{};
    const auto sim_mouse = std::make_shared<Mazemouse::SimMouse>(&sim_game);
    sim_game.addPlugin(sim_mouse);
    sim_game.run();

    return 0;
}
