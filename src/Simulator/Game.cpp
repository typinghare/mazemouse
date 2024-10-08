#include "Game.hpp"

namespace MazemouseSimulator {

Game Game::create(const std::function<void(GameOptions&)>& fn) {
    auto options = GameOptions();
    fn(options);

    return Game(options);
}

void Game::run() const {
    static const auto BACKGROUND_COLOR = sf::Color(240, 235, 216);

    sf::RenderWindow window(
        sf::VideoMode(options_.windowSize.x, options_.windowSize.y),
        WINDOW_CAPTION);
    window.setVerticalSyncEnabled(options_.verticalSyncEnabled);
    window.setFramerateLimit(options_.fps);

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

        window.clear(BACKGROUND_COLOR);
        for (const auto& plugin : plugins_) {
            plugin->draw(window, {});
        }

        window.display();
    }
}

std::shared_ptr<GamePlugin> Game::getPlugin(const std::string& name) const {
    for (const auto& plugin : plugins_) {
        if (plugin->getName() == name) {
            return plugin;
        }
    }

    return nullptr;
}

}  // namespace MazemouseSimulator
