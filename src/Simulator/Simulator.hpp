#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "SimGame.hpp"
#include "SimMouse.hpp"

namespace Mazemouse {

inline void runSimGame() {
    constexpr Maze<16, Cell, Edge> maze{};
    std::cout << maze.isEdgeOpen({0,0}, Direction::UP);

    auto game = SimGame();
    const auto mouse = std::make_shared<SimMouse>(&game);
    game.addPlugin(mouse);
    mouse->start();
    game.run();
}

}  // namespace Mazemouse

#endif
