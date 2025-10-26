#ifndef EFFECTS_ARRANGE_H
#define EFFECTS_ARRANGE_H

#include "Effects/ProjectileEffects/IProjectileEffect.h"
#include "iostream"
#include <vector>
#include <memory>

class EffectsArrange {
    public:
        EffectsArrange() = default;
        ~EffectsArrange() = default;

        void addEffect(std::unique_ptr<IProjectileEffect> effect);
        const std::vector<std::unique_ptr<IProjectileEffect>>& getEffects() const;

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
        std::vector<std::unique_ptr<IProjectileEffect>> _effects;
        int currentEffectIndex = 0;

};


#endif // EFFECTS_ARRANGE_H
