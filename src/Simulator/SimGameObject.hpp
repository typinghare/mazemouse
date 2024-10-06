#ifndef SIM_GAME_OBJECT_HPP
#define SIM_GAME_OBJECT_HPP

#include <SFML/Graphics.hpp>

namespace Mazemouse {

/**
 * Represents a drawable game object.
 */
class SimGameObject : public virtual sf::Drawable {
 public:
    explicit SimGameObject(sf::Vector2u size);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void render();

    sf::Sprite& getSprite();

 protected:
    virtual void renderOnTexture(sf::RenderTexture& render_texture) = 0;

 private:
    sf::Vector2u size_;
    sf::Texture texture_;
    sf::Sprite sprite_;
};

}  // namespace Mazemouse

#endif
