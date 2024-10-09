#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "../Maze/Maze.hpp"

using namespace Mazemouse;

namespace MazemouseSimulator {

constexpr auto WINDOW_CAPTION = "Mazemouse Simulator";
constexpr auto REAL_MAZE_SIDE_LENGTH = 16;

struct GameOptions {
    sf::Vector2u windowSize{};
    int fps{ 30 };
    bool verticalSyncEnabled{ true };
};

class Game;

class GameObject : public virtual sf::Drawable {
 public:
    explicit GameObject(const sf::Vector2u size) : size_(size) {
        texture_.create(size.x, size.y);
        sprite_.setTexture(texture_);
    }

    void draw(sf::RenderTarget& target, const sf::RenderStates states)
        const override {
        target.draw(sprite_, states);
    }

    void render() {
        sf::RenderTexture render_texture;
        render_texture.create(size_.x, size_.y);
        renderOnTexture(render_texture);
        render_texture.display();

        texture_ = render_texture.getTexture();
        sprite_.setTexture(texture_);
    }

    sf::Sprite& getSprite() { return sprite_; }

 protected:
    virtual void renderOnTexture(sf::RenderTexture& render_texture) = 0;

 private:
    sf::Vector2u size_;
    sf::Texture texture_;
    sf::Sprite sprite_;
};

class GamePlugin : public GameObject {
 protected:
    Game* game_;

 public:
    explicit GamePlugin(Game* game);

    ~GamePlugin() override = default;

    virtual std::string getName() = 0;

    virtual void update(int dt) = 0;
};

struct GameCell : Cell {};

struct GameEdge : Edge {
    mutable int num_traveled{ 0 };
};

class Game {
 protected:
    GameOptions options_;

    Maze<REAL_MAZE_SIDE_LENGTH, GameCell, GameEdge> real_maze_;

    std::vector<std::shared_ptr<GamePlugin>> plugins_{};

 public:
    explicit Game(const GameOptions options) : options_(options) {}

    static Game create(const std::function<void(GameOptions&)>& fn);

    [[nodiscard]] GameOptions getOptions() const { return options_; }

    Maze<REAL_MAZE_SIDE_LENGTH, GameCell, GameEdge>& getRealMaze() {
        return real_maze_;
    }

    void run() const;

    template <typename PluginType, typename... Args>
    void usePlugin(Args&&... args) {
        auto plugin =
            std::make_shared<PluginType>(this, std::forward<Args>(args)...);
        plugins_.push_back(plugin);
    }

    template <typename P>
    [[nodiscard]] std::shared_ptr<P> getPlugin(const std::string& name) const;
};

inline GamePlugin::GamePlugin(Game* game) :
    GameObject(game->getOptions().windowSize), game_(game) {}

template <typename P>
std::shared_ptr<P> Game::getPlugin(const std::string& name) const {
    for (const auto& plugin : plugins_) {
        if (plugin->getName() == name) {
            return std::dynamic_pointer_cast<P>(plugin);
        }
    }

    return nullptr;
}

}  // namespace MazemouseSimulator

#endif
