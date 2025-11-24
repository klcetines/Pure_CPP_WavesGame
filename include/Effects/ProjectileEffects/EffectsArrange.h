#ifndef EFFECTS_ARRANGE_H
#define EFFECTS_ARRANGE_H

#include "Effects/ProjectileEffects/IProjectileEffect.h"
#include "Projectiles/Projectile.h"
#include "Enemies/Enemy.h"
#include "iostream"
#include <vector>
#include <memory>

class EffectsArrange {
    public:
        EffectsArrange() = default;
        ~EffectsArrange() = default;

        void addModifier(std::unique_ptr<IProjectileEffect> effect);
        void addImpact(std::unique_ptr<IProjectileEffect> effect);

        const std::vector<std::unique_ptr<IProjectileEffect>>& getModifiers() const;
        const std::vector<std::unique_ptr<IProjectileEffect>>& getImpacts() const;

        void clearEffects();
        void nextEffect();
        bool itsEmpty() const;

        std::unique_ptr<EffectsArrange> Clone() const;
        EffectType GetType() const;

        void OnFire(Projectile& projectile);
        void OnUpdate(Projectile& projectile, float deltaTime);
        ProjectileAction OnImpact(Projectile& projectile, Enemy& enemy);
        void OnDistanceTraveled(Projectile& projectile, float distance);
        void OnExpire(Projectile& projectile);


    private:
        std::vector<std::unique_ptr<IProjectileEffect>> _modifiers; 

        std::vector<std::unique_ptr<IProjectileEffect>> _impacts;

        int currentEffectIndex = 0;
        int currentImpactIndex = 0;
};


#endif // EFFECTS_ARRANGE_H
