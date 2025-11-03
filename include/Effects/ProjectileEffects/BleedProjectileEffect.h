#ifndef BLEED_PROJECTILE_EFFECT_H
#define BLEED_PROJECTILE_EFFECT_H

#include "Effects/ProjectileEffects/IProjectileEffect.h"
#include "Effects/CharacterEffects/ActorEffectsFactory.h"
#include <memory>

class BleedProjectileEffect : public IProjectileEffect {
    public:
        EffectType GetType() const override { return EffectType::Passive; }
        ProjectileAction OnImpact(IActor& enemy) override;
        std::unique_ptr<IProjectileEffect> Clone() const override;
};

#endif // BLEED_PROJECTILE_EFFECT_H