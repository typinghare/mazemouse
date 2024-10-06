#include "SimGameObject.hpp"

namespace Mazemouse {

SimGameObject::SimGameObject(const sf::Vector2u size) : size_(size) {}

void SimGameObject::draw(
    sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite_);
}

sf::Sprite& SimGameObject::getSprite() { return sprite_; }

void SimGameObject::render() {
    sf::RenderTexture render_texture;
    render_texture.create(size_.x, size_.y);
    renderOnTexture(render_texture);
    render_texture.display();

    texture_ = render_texture.getTexture();
    sprite_.setTexture(texture_);
}

}  // namespace Mazemouse
