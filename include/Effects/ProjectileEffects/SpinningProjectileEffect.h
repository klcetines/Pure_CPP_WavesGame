#ifndef SPINNING_PROJECTILE_EFFECT_H
#define SPINNING_PROJECTILE_EFFECT_H

#include "Effects/ProjectileEffects/IProjectileEffect.h"
#include <memory>

class Projectile;

class SpinningProjectileEffect : public IProjectileEffect {
    public:
        EffectType GetType() const override;
        std::string getModifierSymbol() const override;
        uint32_t getColorCode() const override;
        ProjectileAction OnFire(Projectile& projectile, int myIndex);
        ProjectileAction OnUpdate(Projectile& projectile, float deltaTime, int myIndex) override;
        ProjectileAction OnImpact(IActor& enemy) override;
        std::unique_ptr<IProjectileEffect> Clone() const override;
        bool extraImpact() const override;
        
    private:
        int _shotsFired;
        float _degreesSpin;
        float _distUntilShot;
        float _distBetweenShot;
        float _lastDistance;
}; 


#endif //SPINNING_PROJECTILE_EFFECT_H