#include <SFML/Graphics.hpp>
#include <iostream>

// Class that holds a texture and sprite, and handles drawing it.
class GameObject {
public:
    // Default constructor to create a rectangle shape on the texture.
    GameObject() {
        // Define the size of the texture (e.g., 100x100 pixels).
        texture.create(100, 100);

        // Create a rectangle shape to draw on the texture.
        sf::RectangleShape rectangle(sf::Vector2f(100.f, 100.f));
        rectangle.setFillColor(sf::Color::Green); // Set rectangle color to green.

        // Create an off-screen render target to draw the rectangle onto the texture.
        sf::RenderTexture renderTexture;
        renderTexture.create(100, 100);
        renderTexture.clear(sf::Color::Transparent); // Clear with transparent color.
        renderTexture.draw(rectangle);
        renderTexture.display();

        // Copy the render texture to the actual texture.
        texture = renderTexture.getTexture();

        // Set the sprite to use this texture.
        sprite.setTexture(texture);

        // Optional: Set the initial position of the sprite.
        sprite.setPosition(200.f, 150.f);
    }

    // Method to draw the sprite on the given window.
    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

    // Set the position of the sprite.
    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

private:
    sf::Texture texture;  // The texture of the object.
    sf::Sprite sprite;    // The sprite that uses the texture.
};

int main() {
    // Create a window with a title and a size of 800x600 pixels.
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Texture and Sprite Demo");

    // Create a GameObject that draws a green rectangle.
    GameObject gameObject;

    // Main game loop: runs until the window is closed.
    while (window.isOpen()) {
        // Check for events (such as closing the window).
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear the window with a black color.
        window.clear(sf::Color::Black);

        // Draw the game object.
        gameObject.draw(window);

        // Display what has been drawn so far.
        window.display();
    }

    return 0;
}
