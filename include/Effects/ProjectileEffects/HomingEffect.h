#ifndef HOMING_EFFECT_H
#define HOMING_EFFECT_H

#include "Effects/ProjectileEffects/IProjectileEffect.h"
#include <memory>

class HomingEffect : public IProjectileEffect {
    public:
        EffectType GetType() const override;
        ProjectileAction OnImpact(IActor& enemy) override;
        std::unique_ptr<IProjectileEffect> Clone() const override;
};

#endif // HOMING_EFFECT_H