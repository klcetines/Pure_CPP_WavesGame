#include "Projectiles/PlayerProjectile.h"
#include <cmath>
#include <cstdlib>

PlayerProjectile::PlayerProjectile(Vector2f start, Vector2f target, float speed, float damage, float range): position(start), _alive(true), _damage(damage), _maxRange(range) {
    shape.setRadius(6);
    shape.setFillColor(Color::Black);
    shape.setOrigin(6, 6);

    Vector2f dir = target - start;
    float len = sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len != 0)
        velocity = dir / len * speed;
    else
        velocity = {0, 0};

    _collisionBox = CollisionShape(start, 6, 6, 0.0f);
}

void PlayerProjectile::update(float dt) {
    Vector2f movement = velocity * dt;
    
    _lifetime += dt;

    updatePosition(movement);
    updateDistanceTraveled(movement);
    updateCollisionBox();

    if (_traveledDistance >= _maxRange || _lifetime > 5.0f) {
        _alive = false;
    }
}

void PlayerProjectile::draw(RenderWindow& window, float offsetX, float offsetY) {
    shape.setPosition(position.x + offsetX, position.y + offsetY);
    window.draw(shape);
}       

bool PlayerProjectile::isAlive() const {
    return _alive;
}

Vector2f PlayerProjectile::getPosition() const {
    return position;
}

float PlayerProjectile::getDamage() const {
    return _damage;
}

float PlayerProjectile::getSize() const {
    return shape.getRadius();
}

void PlayerProjectile::destroy() {
    _alive = false;
}
CollisionShape PlayerProjectile::getCollisionBox() const {
    return _collisionBox;
}

void PlayerProjectile::updateDistanceTraveled(const Vector2f& movement) {
    _traveledDistance += sqrt(movement.x * movement.x + movement.y * movement.y);
}

void PlayerProjectile::updatePosition(const Vector2f& movement) {
    position += movement;
}

void PlayerProjectile::updateCollisionBox() {
    _collisionBox.center = position;
}