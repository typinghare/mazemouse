#include "SimGame.hpp"

namespace Mazemouse {

void SimGame::run() const {
    sf::RenderWindow window(
        sf::VideoMode(
            SIM_WINDOW_SIDE_LENGTH_PIXEL, SIM_WINDOW_SIDE_LENGTH_PIXEL),
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
        for (const auto& plugin : plugins) {
            plugin->update(dt);
        }

        window.clear(sf::Color(240, 235, 216));
        drawMaze(window);
        drawPeripheralWalls(window);
        window.display();
    }
}

void SimGame::addPlugin(const std::shared_ptr<SimGamePlugin>& plugin) {
    plugins.push_back(plugin);
}

void SimGame::drawMaze(sf::RenderTarget& target) const {
    sf::RenderTexture maze_texture{};
    maze_texture.create(SIM_MAZE_SIDE_LENGTH_PIXEL, SIM_MAZE_SIDE_LENGTH_PIXEL);
    maze_texture.clear(sf::Color::Transparent);
    maze_texture.draw(maze);
    maze_texture.display();

    for (const auto& plugin : plugins) {
        plugin->render();
        plugin->draw(maze_texture, {});
    }

    sf::Sprite maze_sprite(maze_texture.getTexture());
    maze_sprite.setPosition(SIM_WINDOW_MARGIN_PIXEL, SIM_WINDOW_MARGIN_PIXEL);
    target.draw(maze_sprite);
}

void SimGame::drawPeripheralWalls(sf::RenderTarget& target) {
    static const auto COLOR = sf::Color(13, 19, 33);
    static constexpr auto SIM_WALL_THICKNESS_HALF_PIXEL =
        SIM_WALL_THICKNESS_PIXEL / 2;

    sf::RectangleShape rectangle_vertical(
        { SIM_WALL_THICKNESS_PIXEL,
          SIM_MAZE_SIDE_LENGTH_PIXEL + SIM_WALL_THICKNESS_PIXEL });
    rectangle_vertical.setFillColor(COLOR);

    // Left
    rectangle_vertical.setPosition(
        SIM_WINDOW_MARGIN_PIXEL - SIM_WALL_THICKNESS_HALF_PIXEL,
        SIM_WINDOW_MARGIN_PIXEL - SIM_WALL_THICKNESS_HALF_PIXEL);
    target.draw(rectangle_vertical);

    // Right
    rectangle_vertical.setPosition(
        SIM_WINDOW_MARGIN_PIXEL + SIM_MAZE_SIDE_LENGTH_PIXEL -
            SIM_WALL_THICKNESS_HALF_PIXEL,
        SIM_WINDOW_MARGIN_PIXEL - SIM_WALL_THICKNESS_HALF_PIXEL);
    target.draw(rectangle_vertical);

    sf::RectangleShape rectangle_horizontal(
        { SIM_MAZE_SIDE_LENGTH_PIXEL + SIM_WALL_THICKNESS_PIXEL,
          SIM_WALL_THICKNESS_PIXEL });
    rectangle_horizontal.setFillColor(COLOR);

    // Top
    rectangle_horizontal.setPosition(
        SIM_WINDOW_MARGIN_PIXEL - SIM_WALL_THICKNESS_HALF_PIXEL,
        SIM_WINDOW_MARGIN_PIXEL - SIM_WALL_THICKNESS_HALF_PIXEL);
    target.draw(rectangle_horizontal);

    // Down
    rectangle_horizontal.setPosition(
        SIM_WINDOW_MARGIN_PIXEL - SIM_WALL_THICKNESS_HALF_PIXEL,
        SIM_WINDOW_MARGIN_PIXEL + SIM_MAZE_SIDE_LENGTH_PIXEL -
            SIM_WALL_THICKNESS_HALF_PIXEL);
    target.draw(rectangle_horizontal);
}

}  // namespace Mazemouse
