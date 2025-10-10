#include "Character/CharacterStats.h"

CharacterStats::CharacterStats()
    : _life(new Life(100.0f))
    , _speed(5.0f)
    , _damage(25.0f)
    , _bulletsNumber(1)
    , _attackSpeed(1.0f)
    , _projectileSpeed(200.0f)
{
    _projectileEffects = unique_ptr<EffectsArrange> (new EffectsArrange());
    EffectFactory::Initialize();
}

CharacterStats::~CharacterStats() {
    delete _life;
}

void CharacterStats::applyEffect(const Effect& effect) {
    const string& type = effect.getType();
    float value = effect.getValue();

    if (type == "health") {
        _life->heal(value);
    } 
    else if (type == "_damage") {
        _damage = max(1.0f, _damage + value);
    } 
    else if (type == "_speed") {
        _speed = max(0.1f, _speed + value);
    }
    else if (type == "attack_speed") {
        _attackSpeed = max(0.1f, _attackSpeed - value);
    } 
    else if (type == "bullets") {
        _bulletsNumber = max(1, _bulletsNumber + static_cast<int>(value));
    }
    else if (type == "projectile_speed") {
        _projectileSpeed = max(50.0f, _projectileSpeed + value);
    }
    else if (type == "projectile") {
        unique_ptr<IProjectileEffect> newEffect = EffectFactory::Instance().Create(static_cast<int>(value));
        if (newEffect) {
            _projectileEffects->addEffect(std::move(newEffect));
        } 
        else {
            std::cout << "FILE: CharacterStats.cpp \n METHOD: applyEffect \n Unknown projectile effect id '" << static_cast<int>(value) << "'" << std::endl << std::endl;
        }
    }
}
Life& CharacterStats::getLife() {
    return *_life;
}

float CharacterStats::getLifeAmmount() const { return _life->getLife(); }
float CharacterStats::getSpeed() const { return _speed; }
float CharacterStats::getDamage() const { return _damage; }
int CharacterStats::getBulletsNumber() const { return _bulletsNumber; }
float CharacterStats::getAttackSpeed() const { return _attackSpeed; }
float CharacterStats::getProjectileSpeed() const { return _projectileSpeed; }

const EffectsArrange& CharacterStats::getProjectileEffects() const {
    return *_projectileEffects;
}
