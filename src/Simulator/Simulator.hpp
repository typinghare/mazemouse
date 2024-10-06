#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "SimGame.hpp"
#include "SimMouse.hpp"

namespace Mazemouse {

inline void runSimGame() {
    auto game = SimGame();
    const auto mouse = std::make_shared<SimMouse>(&game);
    game.addPlugin(mouse);
    mouse->start();
    game.run();
}

}  // namespace Mazemouse

#endif
