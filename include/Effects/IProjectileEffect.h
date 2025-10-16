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
    Trigger,
    Destroy
};

class IProjectileEffect {
public:
    virtual EffectType GetType() const { return EffectType::Generic; }

    virtual ~IProjectileEffect() = default;

    virtual ProjectileAction OnFire(Projectile& projectile);

    virtual ProjectileAction OnUpdate(Projectile& projectile, float deltaTime);

    virtual ProjectileAction OnImpact(Enemy& enemy) { return ProjectileAction::Destroy; }
    
    virtual ProjectileAction OnDistanceTraveled(Projectile& projectile, float distance);

    virtual ProjectileAction OnExpire(Projectile& projectile);

    virtual std::unique_ptr<IProjectileEffect> Clone() const = 0;
};

#endif // I_PROJECTILE_EFFECT_H