#ifndef CHARACTER_STATS_H
#define CHARACTER_STATS_H

#include "Utils/Life.h"
#include "Utils/Effect.h"
#include "Effects/ProjectileEffects/EffectsArrange.h"
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
    const EffectsArrange& getProjectileEffects() const;
    int getMaxEffectsCount() const;

private:
    Life* _life;
    float _speed;
    float _damage;
    int _bulletsNumber;
    float _attackSpeed;
    float _projectileSpeed;
    std::unique_ptr<EffectsArrange> _projectileEffects;
    int maxEffectsCount = 2;
};

#endif // CHARACTER_STATS_H