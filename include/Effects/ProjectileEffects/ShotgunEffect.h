#ifndef SHOTGUN_PROJECTILE_EFFECT_H
#define SHOTGUN_PROJECTILE_EFFECT_H

#include "Effects/ProjectileEffects/IProjectileEffect.h"
#include "Effects/CharacterEffects/ActorEffectsFactory.h"
#include <memory>

class ShotgunEffect : public IProjectileEffect {
    public:
        EffectType GetType() const override;
        std::string getModifierSymbol() const override;
        uint32_t getColorCode() const override;
        ProjectileAction OnFire(Projectile& projectile, int myIndex) override;
        std::unique_ptr<IProjectileEffect> Clone() const override;

};

#endif // SHOTGUN_PROJECTILE_EFFECT_H