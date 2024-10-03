#include "SimGame.hpp"
#include <SFML/Graphics.hpp>

namespace Mazemouse {

SimGame::SimGame() {
    maze_render_texture_.create(SIM_WINDOW_SIDE_LENGTH_PIXEL, SIM_WINDOW_SIDE_LENGTH_PIXEL);
}

void SimGame::run() {
    sf::RenderWindow window(
        sf::VideoMode(SIM_WINDOW_SIDE_LENGTH_PIXEL, SIM_WINDOW_SIDE_LENGTH_PIXEL),
        SIM_WINDOW_CAPTION);
    window.setVerticalSyncEnabled(SIM_GAME_VERTICAL_SYNC_ENABLED);
    window.setFramerateLimit(SIM_GAME_FPS);

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        const auto dt = clock.restart().asMilliseconds();
        for (const auto& plugin : plugins_) {
            plugin->update(dt);
        }

        // window.clear(sf::Color(43, 45, 66));
        window.clear(sf::Color::White);
        window.draw(renderMaze(maze_render_texture_));
        drawPeripheralWalls(window);
        window.display();
    }
}

SimMaze& SimGame::getMaze() { return maze_; }

void SimGame::addPlugin(const std::shared_ptr<SimGamePlugin>& sim_game_plugin) {
    plugins_.push_back(sim_game_plugin);
}

sf::Sprite SimGame::renderMaze(sf::RenderTexture& maze_layer_texture) {
    maze_layer_texture.clear(sf::Color::Transparent);
    maze_layer_texture.draw(maze_);
    maze_layer_texture.display();

    for (const auto& plugin : plugins_) {
        plugin->draw(maze_render_texture_);
    }

    sf::Sprite mazeSprite(maze_layer_texture.getTexture());
    mazeSprite.setPosition(SIM_WINDOW_MARGIN_PIXEL, SIM_WINDOW_MARGIN_PIXEL);

    return mazeSprite;
}

void SimGame::drawPeripheralWalls(sf::RenderTarget& target) {
    static const auto COLOR = sf::Color::Black;
    static constexpr auto SIM_WALL_THICKNESS_HALF_PIXEL = SIM_WALL_THICKNESS_PIXEL / 2;

    sf::RectangleShape rectangle_vertical(
        { SIM_WALL_THICKNESS_PIXEL, SIM_MAZE_SIDE_LENGTH_PIXEL + SIM_WALL_THICKNESS_PIXEL });
    rectangle_vertical.setFillColor(COLOR);

    // Left
    rectangle_vertical.setPosition(
        SIM_WINDOW_MARGIN_PIXEL - SIM_WALL_THICKNESS_HALF_PIXEL,
        SIM_WINDOW_MARGIN_PIXEL - SIM_WALL_THICKNESS_HALF_PIXEL);
    target.draw(rectangle_vertical);

    // Right
    rectangle_vertical.setPosition(
        SIM_WINDOW_MARGIN_PIXEL + SIM_MAZE_SIDE_LENGTH_PIXEL - SIM_WALL_THICKNESS_HALF_PIXEL,
        SIM_WINDOW_MARGIN_PIXEL - SIM_WALL_THICKNESS_HALF_PIXEL);
    target.draw(rectangle_vertical);

    sf::RectangleShape rectangle_horizontal(
        { SIM_MAZE_SIDE_LENGTH_PIXEL + SIM_WALL_THICKNESS_PIXEL, SIM_WALL_THICKNESS_PIXEL });
    rectangle_horizontal.setFillColor(COLOR);

    // Top
    rectangle_horizontal.setPosition(
        SIM_WINDOW_MARGIN_PIXEL - SIM_WALL_THICKNESS_HALF_PIXEL,
        SIM_WINDOW_MARGIN_PIXEL - SIM_WALL_THICKNESS_HALF_PIXEL);
    target.draw(rectangle_horizontal);

    // Down
    rectangle_horizontal.setPosition(
        SIM_WINDOW_MARGIN_PIXEL - SIM_WALL_THICKNESS_HALF_PIXEL,
        SIM_WINDOW_MARGIN_PIXEL + SIM_MAZE_SIDE_LENGTH_PIXEL - SIM_WALL_THICKNESS_HALF_PIXEL);
    target.draw(rectangle_horizontal);
}

}  // namespace Mazemouse
