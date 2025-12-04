#include "Projectiles/Projectile.h"
#include <cmath>

void Projectile::update(float dt) {
    Vector2f movement = _velocity * dt;
    
    _lifetime += dt;

    updatePosition(movement);
    updateDistanceTraveled(movement);
    updateCollisionBox();

    if (_traveledDistance >= _maxRange || _lifetime > 5.0f) {
        _alive = false;
    }
}

void Projectile::handleImpact(IActor& enemy) {
}

float Projectile::getDamage() const {
    return 0.0f; 
}

float Projectile::getSize() const {
    return 0.0f;
}

Vector2f Projectile::getPosition() const {
    return _position;
}

CollisionShape Projectile::getCollisionBox() const {
    return CollisionShape(_position, 0.0f, 0.0f, 0.0f);
}


void Projectile::updatePosition(const Vector2f& movement) {
    _position += movement;
}

void Projectile::updateDistanceTraveled(const Vector2f& movement) {
    float distance = std::sqrt(movement.x * movement.x + movement.y * movement.y);
    _traveledDistance += distance;
}

void Projectile::updateCollisionBox() {
}