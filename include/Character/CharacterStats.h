#pragma once
#include "Utils/Life.h"
#include "Utils/Effect.h"

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

private:
    Life* _life;
    float _speed;
    float _damage;
    int _bulletsNumber;
    float _attackSpeed;
    float _projectileSpeed;
};