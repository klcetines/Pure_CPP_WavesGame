#ifndef EFFECTS_ARRANGE_H
#define EFFECTS_ARRANGE_H

#include "Effects/ProjectileEffects/IProjectileEffect.h"
#include "Projectiles/PlayerProjectile.h"
#include "Enemies/Enemy.h"
#include "iostream"
#include <vector>
#include <memory>

class EffectsArrange {
    public:
        EffectsArrange();
        ~EffectsArrange();

        void addModifier(std::unique_ptr<IProjectileEffect> effect);
        void addImpact(std::unique_ptr<IProjectileEffect> effect);

        const std::vector<std::unique_ptr<IProjectileEffect>>& getModifiers() const;
        std::vector<std::unique_ptr<IProjectileEffect>>& getModifiers();
        
        const std::vector<std::unique_ptr<IProjectileEffect>>& getImpacts() const;
        std::vector<std::unique_ptr<IProjectileEffect>>& getImpacts();

        void clearEffects();
        bool nextEffect(Projectile& projectile);
        bool modifiersItsEmpty() const;
        bool impactsItsEmpty() const;
        bool modifiersIsFull() const;
        bool impactsIsFull() const;
        int getMaxModifiers() const;
        int getMaxImpacts() const;

        std::unique_ptr<EffectsArrange> Clone() const;
        std::unique_ptr<EffectsArrange> CloneFromIndex(int index) const;
        
        EffectType GetType() const;
        bool hasActiveEffect(EffectType type) const;

        void OnFire(Projectile& projectile);
        void OnUpdate(Projectile& projectile, float deltaTime);
        ProjectileAction OnImpact(Projectile& projectile, Enemy& enemy);
        void OnDistanceTraveled(Projectile& projectile, float distance);
        void OnExpire(Projectile& projectile);
        void swapEffects(int index1, int index2);

        void triggerSpecificEffect(EffectType type, Projectile& projectile);
        void recalcMaxImpacts();


    private:
        std::vector<std::unique_ptr<IProjectileEffect>> _modifiers; 

        std::vector<std::unique_ptr<IProjectileEffect>> _impacts;

        int currentEffectIndex;
        int currentImpactIndex;

        int maxModifiers;
        int maxImpacts;

        bool _pendingOnFire;

        void processImpactEffects(Enemy& enemy);
        ProjectileAction evaluateModifiers(Enemy& enemy, Projectile& projectile);
        ProjectileAction handleTriggeredEffect(Projectile& projectile);

};  


#endif // EFFECTS_ARRANGE_H
