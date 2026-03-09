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
    
    _shape.setRadius(6);
    _shape.setFillColor(Color::Black);
    _shape.setOrigin(6, 6);

    _velocity = normalizeVector(direction) * speed;
    
    _collisionBox = CollisionShape(start, 6, 6, 0.0f);
    
    _hitEnemies = unordered_set<int>();
    
    if (_effects && !_effects->modifiersItsEmpty()) {
        _effects->OnFire(*this);
    }
}

PlayerProjectile::~PlayerProjectile() {
}


void PlayerProjectile::update(float dt, shared_ptr<Enemy> closestEnemy) {
    if (closestEnemy && _effects && _effects->hasActiveEffect(EffectType::Homing)) {
        handleHomingLogic(dt, closestEnemy);
    }
    
    if (_effects && _effects->hasActiveEffect(EffectType::Piercing)) {
        handlePiercingLogic();
    }

    Projectile::update(dt); 

    if (_effects) {
        _effects->OnUpdate(*this, dt);
    }
    
    _shape.setPosition(_position);
    _collisionBox.center = _position;

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
    if (_lastHitEnemy != nullptr) {
        if (!_collisionBox.intersects(_lastHitEnemy->getCollisionBox())) {
            _effects->triggerSpecificEffect(EffectType::Piercing, *this);
            _lastHitEnemy = nullptr; 
        }
    }
}     

void PlayerProjectile::handleImpact(IActor& animatedObject) {
    Enemy* enemy = dynamic_cast<Enemy*>(&animatedObject);
    
    if (!enemy) return;
    
    int enemyId = enemy->getId();

    if (_hitEnemies.find(enemyId) != _hitEnemies.end()) {
        return;
    }

    _hitEnemies.insert(enemyId);
    _lastHitEnemy = enemy;
    
    enemy->takeDamage(_damage);
    if(enemy->getLife() <= 0){
        _lastHitEnemy = nullptr;
    }

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

float PlayerProjectile::getVectorLength(const Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

Vector2f PlayerProjectile::normalizeVector(const Vector2f& v) {
    float len = getVectorLength(v);
    if (len != 0) return v / len;
    return Vector2f(0, 0);
}

void PlayerProjectile::draw(RenderWindow& window, float offsetX, float offsetY) {
    _shape.setPosition(_position.x + offsetX, _position.y + offsetY);
    window.draw(_shape);
}       

CollisionShape PlayerProjectile::getCollisionBox() const { 
    return _collisionBox; 
}

void PlayerProjectile::destroy() {
    _alive = false;
}