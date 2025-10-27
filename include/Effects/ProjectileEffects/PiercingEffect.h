#ifndef PIERCING_EFFECT_H
#define PIERCING_EFFECT_H

#include "Effects/ProjectileEffects/IProjectileEffect.h"
#include <memory>

class PiercingEffect : public IProjectileEffect {
    public:
        ProjectileAction OnImpact(IAnimatedObject& enemy) override;
        std::unique_ptr<IProjectileEffect> Clone() const override;
};

#endif // PIERCING_EFFECT_H