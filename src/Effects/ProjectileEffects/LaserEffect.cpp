#include "Effects/ProjectileEffects/LaserEffect.h"
#include "Projectiles/PlayerProjectile.h"

float getVectorLength(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f normalizeVector(const sf::Vector2f& v) {
    float len = getVectorLength(v);
    if (len != 0) return v / len;
    return sf::Vector2f(0, 0);
}

EffectType LaserEffect::GetType() const{
    return EffectType::Laser;
}

std::string LaserEffect::getModifierSymbol() const{
    return "L";
}

uint32_t LaserEffect::getColorCode() const{
    return 0xff8a1ddd;
}

ProjectileAction LaserEffect::OnFire(Projectile& projectile){
    _startingPoint = projectile.getPosition();
    
    sf::Vector2f direction = normalizeVector(projectile.getVelocity());
    
    _lastPoint = _startingPoint + direction * projectile.getMaxRange();
    
    float speed = getVectorLength(projectile.getVelocity());
    if (speed > 0) {
        _maxLifetime = projectile.getMaxRange() / speed;
    } else {
        _maxLifetime = 0.1f;
    }
    
    _lifetime = 0.0f;
    
    return ProjectileAction::Continue;
}

ProjectileAction LaserEffect::OnUpdate(Projectile& projectile, float deltaTime, int myIndex) {
    _lifetime += deltaTime;
    
    if (_lifetime >= _maxLifetime) {
        return ProjectileAction::Destroy;
    }

    return ProjectileAction::Continue;
}

ProjectileAction LaserEffect::OnImpact(IActor& enemy) {
    return ProjectileAction::Continue;
}

std::unique_ptr<IProjectileEffect> LaserEffect::Clone() const{
    return std::make_unique<LaserEffect>(*this);
}

bool LaserEffect::extraImpact() const {
    return true;
}