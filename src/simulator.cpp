#include "Simulator.hpp"

using namespace MazemouseSimulator;

int main() {
    auto game = Game::create([](auto& options) {
        const auto [x, y] = MazePlugin::getWindowSize();
        options.windowSize.x = x;
        options.windowSize.y = y;

        options.fps = 60;
    });
    game.usePlugin<FloorMazePlugin>();
    game.usePlugin<WallMazePlugin>();
    game.usePlugin<PeripheralWallMazePlugin>();
    game.usePlugin<MouseMazePlugin>();
    game.usePlugin<StateDisplayMazePlugin>();
    game.run();

    return 0;
}
