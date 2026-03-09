#ifndef HOMING_EFFECT_H
#define HOMING_EFFECT_H

#include "Effects/ProjectileEffects/IProjectileEffect.h"
#include <memory>

class HomingEffect : public IProjectileEffect {
    public:
        EffectType GetType() const override;
        std::string getModifierSymbol() const override;
        uint32_t getColorCode() const override;
        ProjectileAction OnImpact(IActor& enemy) override;
        std::unique_ptr<IProjectileEffect> Clone() const override;
        bool isPassive() const override;
};

#endif // HOMING_EFFECT_H