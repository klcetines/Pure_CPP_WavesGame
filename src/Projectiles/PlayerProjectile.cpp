#include "Projectiles/PlayerProjectile.h"
#include <cmath>
#include <cstdlib>

PlayerProjectile::PlayerProjectile(Vector2f start, Vector2f target, float speed, float damage, const EffectsArrange& effects, float range)
    : position(start), _alive(true), _damage(damage), _maxRange(range), _traveledDistance(0.f), _effects(effects.Clone()){
    shape.setRadius(6);
    shape.setFillColor(Color::Black);
    shape.setOrigin(6, 6);

    Vector2f catets = target - start;
    float hipotenusa = sqrt(catets.x * catets.x + catets.y * catets.y);
    if (hipotenusa != 0)
        velocity = catets / hipotenusa * speed;
    else
        velocity = {0, 0};

    _collisionBox = CollisionShape(start, 6, 6, 0.0f);
    
    _hitEnemies = unordered_set<int>();
    
    if (_effects && !_effects->itsEmpty()) {
        _effects->OnFire(*this);
    }
}

PlayerProjectile::~PlayerProjectile() {
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
    if (closest_enemy && _effects && (_effects->GetType() == EffectType::Homing)) {
        Vector2f targetDirection = closest_enemy->getHeadPosition() - position;
        float length = sqrt(targetDirection.x * targetDirection.x + targetDirection.y * targetDirection.y);
        
        if (length != 0) {
            targetDirection /= length;
            updateVelocityTowardsTarget(targetDirection, dt);
        }
    }
    _effects->OnUpdate(*this, dt);
    
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

void PlayerProjectile::handleImpact(IAnimatedObject& animatedObject) {
    Enemy* enemyPtr = dynamic_cast<Enemy*>(&animatedObject);

    if (enemyPtr) {
        Enemy& enemy = *enemyPtr; 
        int enemyId = enemy.getId();
        if (_hitEnemies.find(enemyId) != _hitEnemies.end()) {
            return;
        }

        _hitEnemies.insert(enemyId);
        enemy.takeDamage(_damage);

        if (_effects && !_effects->itsEmpty()) {
            ProjectileAction result = _effects->OnImpact(*this, enemy);

            if (result == ProjectileAction::Destroy) {
                destroy();
            }
        } 
        else {
            destroy();
        }
    }
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
    _effects->OnDistanceTraveled(*this, _traveledDistance);
}

void PlayerProjectile::updatePosition(const Vector2f& movement) {
    position += movement;
}

void PlayerProjectile::updateCollisionBox() {
    _collisionBox.center = position;
}

void PlayerProjectile::updateVelocityTowardsTarget(const Vector2f& targetDirection, float deltaTime) {
    float speed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    float homingFactor = 5.0f;

    Vector2f currentDir = velocity / speed;

    Vector2f newDir;
    newDir.x = currentDir.x + (targetDirection.x - currentDir.x) * homingFactor * deltaTime;
    newDir.y = currentDir.y + (targetDirection.y - currentDir.y) * homingFactor * deltaTime;

    float len = sqrt(newDir.x * newDir.x + newDir.y * newDir.y);
    if (len != 0) {
        newDir /= len;
        velocity = newDir * speed;
    }
}
