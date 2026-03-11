#ifndef LASER_PROJECTILE_EFFECT_H
#define LASER_PROJECTILE_EFFECT_H

#include "Effects/ProjectileEffects/IProjectileEffect.h"
#include "Effects/CharacterEffects/ActorEffectsFactory.h"
#include <memory>

class LaserEffect : public IProjectileEffect {
    public:
        EffectType GetType() const override;
        std::string getModifierSymbol() const override;
        uint32_t getColorCode() const override;
        ProjectileAction OnFire(Projectile& projectile);
        ProjectileAction OnUpdate(Projectile& projectile, float deltaTime, int myIndex) override;
        ProjectileAction OnImpact(IActor& enemy) override;
        std::unique_ptr<IProjectileEffect> Clone() const override;
        bool extraImpact() const override;
    private:
        Vector2f _startingPoint;
        Vector2f _lastPoint;
        float _lifetime;
        float _maxLifetime;

};

#endif // LASER_PROJECTILE_EFFECT_H