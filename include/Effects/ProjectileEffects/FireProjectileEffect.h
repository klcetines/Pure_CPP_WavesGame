#ifndef FIRE_PROJECTILE_EFFECT_H
#define FIRE_PROJECTILE_EFFECT_H

#include "Effects/ProjectileEffects/IProjectileEffect.h"
#include "Effects/CharacterEffects/ActorEffectsFactory.h"
#include <memory>

class FireProjectileEffect : public IProjectileEffect {
    public:
        EffectType GetType() const override;
        ProjectileAction OnImpact(IActor& enemy) override;
        std::unique_ptr<IProjectileEffect> Clone() const override;
};

#endif // FIRE_PROJECTILE_EFFECT_H