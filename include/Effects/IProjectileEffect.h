#ifndef I_PROJECTILE_EFFECT_H
#define I_PROJECTILE_EFFECT_H

#include "Enemies/Enemy.h"
#include <memory>

class Projectile;

enum class EffectType {
    Generic,
    Homing,
    Piercing
};

enum class ProjectileAction {
    Continue,
    Destroy
};

class IProjectileEffect {
public:
    virtual EffectType GetType() const { return EffectType::Generic; }

    virtual ~IProjectileEffect() = default;

    virtual void OnFire(Projectile& projectile) {}

    virtual void OnUpdate(Projectile& projectile, float deltaTime) {}

    virtual ProjectileAction OnImpact(Enemy& enemy) { return ProjectileAction::Destroy; }
    
    virtual void OnDistanceTraveled(Projectile& projectile, float distance) {}

    virtual void OnExpire(Projectile& projectile) {}

    virtual std::unique_ptr<IProjectileEffect> Clone() const = 0;
};

#endif // I_PROJECTILE_EFFECT_H