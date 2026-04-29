#include "Utils/Camera.h"

Camera::Camera(sf::Vector2f size) : screenSize(size), position(size / 2.f) {}

void Camera::follow(const sf::Vector2f targetPos) {
    position = targetPos;
}

sf::Vector2f Camera::worldToScreen(sf::Vector2f worldPos) const {
    sf::Vector2f offset = (screenSize / 2.f) - position;
    return worldPos + offset;
}

sf::FloatRect Camera::getBounds() const {
    return sf::FloatRect(position - (screenSize / 2.f), screenSize);
}

sf::Vector2f Camera::getOffset() const {
    return (screenSize / 2.f) - position;
}