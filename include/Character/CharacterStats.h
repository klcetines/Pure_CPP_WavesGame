#ifndef CHARACTER_STATS_H
#define CHARACTER_STATS_H

#include "Utils/Life.h"
#include "Utils/Effect.h"
#include "Utils/EquipContext.h"
#include "Effects/ProjectileEffects/EffectsFactory.h"
#include <iostream>


class CharacterStats {
public:
    CharacterStats();
    ~CharacterStats();
    
    void applyEffect(const Effect& effect);
    Life& getLife();  
    float getLifeAmmount() const;
    float getSpeed() const;
    float getDamage() const;
    int getBulletsNumber() const;
    float getAttackSpeed() const;
    float getProjectileSpeed() const;
    EffectsArrange& getProjectileEffects();
    int getMaxEffectsCount() const;
    EquipContext getEquipContext();

private:
    Life* _life;
    float _speed;
    float _damage;
    int _bulletsNumber;
    float _attackSpeed;
    float _projectileSpeed;
    std::unique_ptr<EffectsArrange> _projectileEffects;
    std::unique_ptr<EffectsInventory> _effectsInventory;
    int maxEffectsCount = 2;

    bool handleAddImpact(std::unique_ptr<IProjectileEffect> newEffect);
    bool handleAddModifier(std::unique_ptr<IProjectileEffect> newEffect);
};

#endif // CHARACTER_STATS_H