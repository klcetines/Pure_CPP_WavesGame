#ifndef PIERCING_EFFECT_H
#define PIERCING_EFFECT_H

#include "Effects/IProjectileEffect.h"
#include <memory>

class PiercingEffect : public IProjectileEffect {
    public:
        void OnImpact(Projectile& projectile, Enemy& enemy) override;
        std::unique_ptr<IProjectileEffect> Clone() const override;
};

#endif // PIERCING_EFFECT_H