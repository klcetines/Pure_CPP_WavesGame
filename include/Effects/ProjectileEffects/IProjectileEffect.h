#ifndef I_PROJECTILE_EFFECT_H
#define I_PROJECTILE_EFFECT_H

#include "Utils/IActor.h"
#include <memory>

class Projectile;

enum class EffectType {
    Generic,
    Homing,
    Piercing,
    Passive
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

    virtual ProjectileAction OnFire(Projectile& projectile) { return ProjectileAction::Continue; }

    virtual ProjectileAction OnUpdate(Projectile& projectile, float deltaTime) { return ProjectileAction::Continue; };

    virtual ProjectileAction OnImpact(IActor& enemy) { return ProjectileAction::Destroy; }
    
    virtual ProjectileAction OnDistanceTraveled(Projectile& projectile, float distance) { return ProjectileAction::Destroy; }

    virtual ProjectileAction OnExpire(Projectile& projectile) { return ProjectileAction::Destroy; };

    virtual std::unique_ptr<IProjectileEffect> Clone() const = 0;
};

#endif // I_PROJECTILE_EFFECT_H