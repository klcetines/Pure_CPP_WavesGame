#include "Projectiles/PlayerProjectile.h"
#include <cmath>
#include <cstdlib>

PlayerProjectile::PlayerProjectile(Vector2f start, Vector2f target, float speed, float damage, ProjectileEffectsList* effects, float range)
    : position(start), _alive(true), _damage(damage), _maxRange(range) {

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
    _effects = effects;
    _effects->_current = _effects->getFirst();
}

void PlayerProjectile::update(float dt){
    Vector2f movement = velocity * dt;
    
    _lifetime += dt;

    updatePosition(movement);
    updateDistanceTraveled(movement);
    updateCollisionBox();

    if (_traveledDistance >= _maxRange || _lifetime > 5.0f) {
        _alive = false;
    }
}

void PlayerProjectile::update(float dt, shared_ptr<Enemy> closest_enemy) {
    if (_effects->_current && _effects->_current->effect->checkEffect("AUTOAIM") && closest_enemy) {
        Vector2f targetDirection = closest_enemy ? closest_enemy->getPosition() - position : velocity;
        updateVelocityTowardsTarget(targetDirection);
    }
    
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
    if(getCurrentEffectTrigger() == 'D' && (_traveledDistance > _maxRange/2)) {

        _effects->_current = _effects->_current->next;
    }
}

void PlayerProjectile::updatePosition(const Vector2f& movement) {
    position += movement;
}

void PlayerProjectile::updateCollisionBox() {
    _collisionBox.center = position;
}

void PlayerProjectile::updateVelocityTowardsTarget(const Vector2f& targetDirection) {
    float speed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    float rotationSpeed = 5.0f;
    
    Vector2f currentDir = velocity / speed;
    
    Vector2f newDir;
    newDir.x = currentDir.x + (targetDirection.x - currentDir.x) * rotationSpeed;
    newDir.y = currentDir.y + (targetDirection.y - currentDir.y) * rotationSpeed;
    
    float len = sqrt(newDir.x * newDir.x + newDir.y * newDir.y);
    if (len != 0) {
        newDir /= len;
        velocity = newDir * speed;
    }
}

char PlayerProjectile::getCurrentEffectTrigger() const {
    if (_effects == nullptr || _effects->_current == nullptr) return '\0';
    return _effects->_current->effect->getTriggeredBy();
}