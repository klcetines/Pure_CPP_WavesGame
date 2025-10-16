#ifndef HOMING_EFFECT_H
#define HOMING_EFFECT_H

#include "Effects/IProjectileEffect.h"
#include <memory>

class HomingEffect : public IProjectileEffect {
    public:
        EffectType GetType() const override;
        ProjectileAction OnImpact(Enemy& enemy) override;
        std::unique_ptr<IProjectileEffect> Clone() const override;
};

#endif // HOMING_EFFECT_H