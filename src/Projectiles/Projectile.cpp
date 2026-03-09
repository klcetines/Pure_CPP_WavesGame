#include "Projectiles/Projectile.h"
#include "Effects/ProjectileEffects/EffectsArrange.h"
#include <cmath>

Projectile::Projectile() {
    _effects = nullptr;
}

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

bool Projectile::isAlive() const {
    return _alive;
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

Vector2f Projectile::getVelocity() const {
    return _velocity;
}

CollisionShape Projectile::getCollisionBox() const {
    return CollisionShape(_position, 0.0f, 0.0f, 0.0f);
}

float Projectile::getTraveledDistance() const{
    return _traveledDistance;
}

float Projectile::getMaxRange() const{
    return _maxRange;
}

const EffectsArrange& Projectile::getEffects() const {
    if (_effects) {
        return *_effects;
    } 
    else {
        static EffectsArrange emptyEffects;
        return emptyEffects;
    }
}

void Projectile::setSpawnCallback(SpawnCallback callback) {
    _spawnCallback = callback;
}

void Projectile::spawnChild(std::shared_ptr<Projectile> child) {
    if (_spawnCallback) {
        _spawnCallback(child);
    }
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