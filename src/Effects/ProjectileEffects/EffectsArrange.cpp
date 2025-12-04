#include "Effects/ProjectileEffects/EffectsArrange.h"

void EffectsArrange::addModifier(std::unique_ptr<IProjectileEffect> effect){
    if (effect) {
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

std::unique_ptr<EffectsArrange> EffectsArrange::Clone() const {
    auto clone = std::make_unique<EffectsArrange>();
    for (const auto& effect : _modifiers) {
        clone->addModifier(effect->Clone());
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
    cout << "EffectsArrange::OnImpact called" << endl;
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

