#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

class Camera {
public:
    Camera(sf::Vector2f size);

    void follow(const sf::Vector2f targetPos);

    sf::Vector2f worldToScreen(sf::Vector2f worldPos) const;

    sf::FloatRect getBounds() const;
    
    sf::Vector2f getOffset() const;

private:
    sf::Vector2f position;
    sf::Vector2f screenSize;

};

#endif // CAMERA_H