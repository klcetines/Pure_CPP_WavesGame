#include "Effects/ProjectileEffects/EffectsArrange.h"

EffectsArrange::EffectsArrange(){
    maxModifiers = 2;
    maxImpacts = 1;
    
    currentEffectIndex = 0;
    currentImpactIndex = 0;

    _modifiers.reserve(maxModifiers);
    _impacts.reserve(maxImpacts);
}

EffectsArrange::~EffectsArrange(){
    _modifiers.clear();
    _impacts.clear();
}

void EffectsArrange::addModifier(std::unique_ptr<IProjectileEffect> effect){
    if (effect) {
        if(effect->extraImpact()){
            maxImpacts++;
        }
        _modifiers.push_back(std::move(effect));
    }
}

const std::vector<std::unique_ptr<IProjectileEffect>>& EffectsArrange:: getModifiers() const{
    return _modifiers;
}

void EffectsArrange::addImpact(std::unique_ptr<IProjectileEffect> effect){
    if (effect) {
        _impacts.push_back(std::move(effect));
    }
}

const std::vector<std::unique_ptr<IProjectileEffect>>& EffectsArrange:: getImpacts() const{
    return _impacts;
}

void EffectsArrange::clearEffects(){
    _modifiers.clear();
    currentEffectIndex = 0;

    _impacts.clear();
    currentImpactIndex = 0;
}

void EffectsArrange::nextEffect(){
    if (!_modifiers.empty() && currentEffectIndex < _modifiers.size()) {
        currentEffectIndex++;
    }
    
}

bool EffectsArrange::modifiersItsEmpty() const{
    return _modifiers.empty();
}

bool EffectsArrange::impactsItsEmpty() const{
    return _impacts.empty();
}

bool EffectsArrange::modifiersIsFull() const{
    return _modifiers.size() >= maxModifiers;
}

bool EffectsArrange::impactsIsFull() const{
    return _impacts.size() >= maxImpacts;
}

int EffectsArrange::getMaxModifiers() const{
    return maxModifiers;
}

int EffectsArrange::getMaxImpacts() const{
    return maxImpacts;
}

std::unique_ptr<EffectsArrange> EffectsArrange::Clone() const {
    auto clone = std::make_unique<EffectsArrange>();
    for (const auto& effect : _modifiers) {
        clone->addModifier(effect->Clone());
    }
    for (const auto& effect : _impacts) {
        clone->addImpact(effect->Clone());
    }
    return clone;
}

EffectType EffectsArrange::GetType() const {
    if (!_modifiers.empty() && currentEffectIndex < _modifiers.size()) {
        return _modifiers[currentEffectIndex]->GetType();
    }
    else return EffectType::Generic; 
}

void EffectsArrange::OnFire(Projectile& projectile){
    if (!_modifiers.empty() && currentEffectIndex < _modifiers.size()) {
        _modifiers[currentEffectIndex]->OnFire(projectile);
    }
}

void EffectsArrange::OnUpdate(Projectile& projectile, float deltaTime){
    if (!_modifiers.empty() && currentEffectIndex < _modifiers.size()) {
        if(_modifiers[currentEffectIndex]->OnUpdate(projectile, deltaTime) == ProjectileAction::Trigger){
            nextEffect();
        }
    }
}

ProjectileAction EffectsArrange::OnImpact(Projectile& projectile, Enemy& enemy){
    if (currentImpactIndex < _impacts.size()) {
        _impacts[currentImpactIndex]->OnImpact(enemy);
        currentImpactIndex++; 
    }

    if (_modifiers.empty() || currentEffectIndex >= _modifiers.size()) {
        return ProjectileAction::Destroy;
    }

    auto& currentMod = _modifiers[currentEffectIndex];
    ProjectileAction action = currentMod->OnImpact(enemy);

    if (action == ProjectileAction::Trigger) {
        nextEffect(); 
        if (currentEffectIndex >= _modifiers.size()) {
            return ProjectileAction::Destroy;
        }
        return ProjectileAction::Continue;
    }
        
    return action;
}

void EffectsArrange::OnDistanceTraveled(Projectile& projectile, float distance){
    if (!_modifiers.empty() && currentEffectIndex < _modifiers.size()) {
       if (_modifiers[currentEffectIndex]->OnDistanceTraveled(projectile, distance ) == ProjectileAction::Trigger) {
            nextEffect();
        }
    }
}

void EffectsArrange::OnExpire(Projectile& projectile){
    if (!_modifiers.empty() && currentEffectIndex < _modifiers.size()) {
        if(_modifiers[currentEffectIndex]->OnExpire(projectile) == ProjectileAction::Trigger) {
            nextEffect();
        }
    }
}

void EffectsArrange::swapEffects(int index1, int index2) {
    int totalEffects = _modifiers.size() + _impacts.size();

    if (index1 < 0 || index1 >= totalEffects || index2 < 0 || index2 >= totalEffects) {
        return; 
    }

    std::unique_ptr<IProjectileEffect>* effect1;
    std::unique_ptr<IProjectileEffect>* effect2;

    if (index1 < _modifiers.size()) {
        effect1 = &_modifiers[index1];
    } 
    else {
        effect1 = &_impacts[index1 - _modifiers.size()];
    }

    if (index2 < _modifiers.size()) {
        effect2 = &_modifiers[index2];
    } 
    else {
        effect2 = &_impacts[index2 - _modifiers.size()];
    }

    std::swap(*effect1, *effect2);
}

