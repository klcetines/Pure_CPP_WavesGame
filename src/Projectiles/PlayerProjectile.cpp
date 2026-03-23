#include "Projectiles/PlayerProjectile.h"
#include <cmath>
#include <cstdlib>

PlayerProjectile::PlayerProjectile(Vector2f start, Vector2f direction, float speed, float damage, const EffectsArrange& effects, float range)
    : _damage(damage)
{
    _position = start;
    _maxRange = range;
    _traveledDistance = 0.f;
    _alive = true;
    _lifetime = 0.f;
    _effects = effects.Clone();

    _velocity = normalizeVector(direction) * speed;
    
    _collisionBox = CollisionShape(start, 6.0f);
    createShapeFromEffects();
    
    _hitEnemies = unordered_set<int>();
}

PlayerProjectile::~PlayerProjectile() {
}

void PlayerProjectile::setSpawnCallback(SpawnCallback callback) {
    Projectile::setSpawnCallback(callback);
    
    if (_effects && !_effects->modifiersItsEmpty()) {
        _effects->OnFire(*this);
    }
    updateCollisionBox();
}

void PlayerProjectile::update(float dt, shared_ptr<Enemy> closestEnemy) {
    if (closestEnemy && _effects && _effects->hasActiveEffect(EffectType::Homing)) {
        handleHomingLogic(dt, closestEnemy);
        float angle = atan2(_velocity.y, _velocity.x) * 180.f / 3.14159265f;
        _shape->setRotation(angle);
    }
    
    if (_effects && _effects->hasActiveEffect(EffectType::Piercing)) {
        handlePiercingLogic();
    }

    if(_effects && _effects->hasActiveEffect(EffectType::Laser)) {
        float angle = atan2(_velocity.y, _velocity.x) * 180.f / 3.14159265f;
        _shape->setRotation(angle);
        _lifetime += dt;
    }
    else {
        Projectile::update(dt);
    }


    if (_effects) {
        _effects->OnUpdate(*this, dt);
    }

    if (_shape) {
        _shape->setPosition(_position);
    }

    if (_traveledDistance >= _maxRange || _lifetime > _maxLifetime) {
        _alive = false;
    }
}

void PlayerProjectile::handleHomingLogic(float dt, shared_ptr<Enemy> target) {
    Vector2f targetPos = target->getHeadPosition();
    Vector2f directionToTarget = targetPos - _position;
    
    float dist = getVectorLength(directionToTarget);
    if (dist == 0) return;
    Vector2f desiredDir = directionToTarget / dist;

    float currentSpeed = getVectorLength(_velocity);
    Vector2f currentDir = (currentSpeed != 0) ? _velocity / currentSpeed : Vector2f(0,0);

    float homingFactor = 7.5f; // Speed of homing adjustment
    Vector2f newDir = currentDir + (desiredDir - currentDir) * homingFactor * dt;
    
    _velocity = normalizeVector(newDir) * currentSpeed;
}

void PlayerProjectile::handlePiercingLogic() {
    auto target = _lastHitEnemy.lock();
    if (!target) return;
    
    if (!_collisionBox.intersects(target->getCollisionBox())) {
        _lastHitEnemy.reset();
        _effects->nextEffect(*this);
    }
}     

void PlayerProjectile::handleImpact(std::shared_ptr<IActor> animatedObject) {
    std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(animatedObject);
    if (!enemy) return;

    int enemyId = enemy->getId();
    bool isLaser = _effects && _effects->hasActiveEffect(EffectType::Laser);

    if (!isLaser) {
        if (_hitEnemies.count(enemyId)) return;
        _hitEnemies.insert(enemyId);
    }
    float finalDamage = isLaser ? (_damage * 0.03f) : _damage;
    enemy->takeDamage(finalDamage, isLaser);

    _lastHitEnemy = enemy;
    if(!isLaser){ 
        if (_effects && (!_effects->modifiersItsEmpty() || !_effects->impactsItsEmpty())) {
            ProjectileAction result = _effects->OnImpact(*this, *enemy);
            if (result == ProjectileAction::Destroy) {
                destroy();
            }
        } 
        else {
            destroy();
        }
    }
}

void PlayerProjectile::createShapeFromEffects() {
    if (_effects && _effects->hasActiveEffect(EffectType::Laser)) {
        auto rect = std::make_unique<sf::RectangleShape>(sf::Vector2f(_maxRange, 12.f));
        
        rect->setOrigin(0.f, 6.f); 
        rect->setFillColor(sf::Color::Red);

        _shape = std::move(rect);
        float angle = atan2(_velocity.y, _velocity.x) * 180.f / 3.14159265f;
        _shape->setRotation(angle);
    }
    else if (_effects && _effects->hasActiveEffect(EffectType::Piercing)) {
        auto tri = std::make_unique<sf::ConvexShape>(3);
        tri->setPoint(0, {0, -10});
        tri->setPoint(1, {20, 0});
        tri->setPoint(2, {0, 10});
        tri->setOrigin(10, 0);
        tri->setFillColor(sf::Color::Yellow);

        _shape = std::move(tri);
    }
    else {
        auto circle = std::make_unique<sf::CircleShape>(6);
        circle->setOrigin(6, 6);
        circle->setFillColor(sf::Color::Black);

        _shape = std::move(circle);
    }
}

float PlayerProjectile::getVectorLength(const Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

Vector2f PlayerProjectile::normalizeVector(const Vector2f& v) {
    float len = getVectorLength(v);
    if (len != 0) return v / len;
    return Vector2f(0, 0);
}

void PlayerProjectile::draw(RenderWindow& window, float offsetX, float offsetY) {
    _shape->setPosition(_position.x + offsetX, _position.y + offsetY);
    window.draw(*_shape);
}       

CollisionShape PlayerProjectile::getCollisionBox() const { 
    return _collisionBox; 
}

float PlayerProjectile::getDamage() const {
    return _damage;
}

void PlayerProjectile::updateCollisionBox(){
    if(_effects && _effects->hasActiveEffect(EffectType::Laser)) {
        sf::Vector2f direction = normalizeVector(_velocity);
        float length = _maxRange;
        
        sf::Vector2f capsuleCenter = _position + direction * (length / 2.0f);
        
        float rotation = atan2(direction.y, direction.x) * 180.0f / 3.14159265f;
        
        _collisionBox = CollisionShape(capsuleCenter, 6.0f, length, rotation);
    } 
    else {
        _collisionBox.center = _position;
        _collisionBox.radius = 6; 
    }
}

void PlayerProjectile::destroy() {
    _alive = false;
}