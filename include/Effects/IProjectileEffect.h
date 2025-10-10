#ifndef I_PROJECTILE_EFFECT_H
#define I_PROJECTILE_EFFECT_H

#include "Projectiles/Projectile.h"
#include "Enemies/Enemy.h"
#include <memory>

enum class EffectType {
    Generic,
    Homing,
    Piercing
};

class IProjectileEffect {
public:
    virtual EffectType GetType() const { return EffectType::Generic; }

    virtual ~IProjectileEffect() = default;

    virtual void OnFire(Projectile& projectile) {}

    virtual void OnUpdate(Projectile& projectile, float deltaTime) {}

    virtual void OnImpact(Projectile& projectile, Enemy& enemy) {}

    virtual void OnDistanceTraveled(Projectile& projectile, float distance) {}

    virtual void OnExpire(Projectile& projectile) {}

    virtual std::unique_ptr<IProjectileEffect> Clone() const = 0;
};

#endif // I_PROJECTILE_EFFECT_H