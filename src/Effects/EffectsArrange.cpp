#include "Effects/EffectsArrange.h"

void EffectsArrange::addEffect(std::unique_ptr<IProjectileEffect> effect){
    _effects.push_back(std::move(effect));
}

const std::vector<std::unique_ptr<IProjectileEffect>>& EffectsArrange:: getEffects() const{
    return _effects;
}

void EffectsArrange::clearEffects(){
    _effects.clear();
    currentEffectIndex = 0;
}
void EffectsArrange::nextEffect(){
    if (!_effects.empty() && currentEffectIndex < _effects.size()-1) {
        currentEffectIndex = currentEffectIndex+1;
    }
    
}
bool EffectsArrange::itsEmpty() const{
    return _effects.empty();
}

std::unique_ptr<EffectsArrange> EffectsArrange::Clone() const {
    auto clone = std::make_unique<EffectsArrange>();
    for (const auto& effect : _effects) {
        clone->addEffect(effect->Clone());
    }
    return clone;
}

EffectType EffectsArrange::GetType() const {
    if (!_effects.empty()) {
        return _effects[currentEffectIndex]->GetType();
    }
    else return EffectType::Generic; 
}

void EffectsArrange::OnFire(Projectile& projectile){
    if (!_effects.empty() && currentEffectIndex < _effects.size()) {
        _effects[currentEffectIndex]->OnFire(projectile);
    }
}

void EffectsArrange::OnUpdate(Projectile& projectile, float deltaTime){
    if (!_effects.empty() && currentEffectIndex < _effects.size()) {
        if(_effects[currentEffectIndex]->OnUpdate(projectile, deltaTime) == ProjectileAction::Trigger){
            nextEffect();
        }
    }
}

ProjectileAction EffectsArrange::OnImpact(Projectile& projectile, Enemy& enemy){
    if (!_effects.empty() && currentEffectIndex < _effects.size()) {
        if (_effects[currentEffectIndex]->OnImpact(enemy) == ProjectileAction::Trigger) {
            nextEffect();
            if(_effects[currentEffectIndex-1]->GetType() == EffectType::Homing){
                return ProjectileAction::Continue;
            }
            else{
                return ProjectileAction::Destroy;
            }
        }
    }
    return ProjectileAction::Destroy;
}

void EffectsArrange::OnDistanceTraveled(Projectile& projectile, float distance){
    if (!_effects.empty() && currentEffectIndex < _effects.size()) {
       if (_effects[currentEffectIndex]->OnDistanceTraveled(projectile, distance ) == ProjectileAction::Trigger) {
            nextEffect();
        }
    }
}

void EffectsArrange::OnExpire(Projectile& projectile){
    if (!_effects.empty() && currentEffectIndex < _effects.size()) {
        if(_effects[currentEffectIndex]->OnExpire(projectile) == ProjectileAction::Trigger) {
            nextEffect();
        }
    }
}

