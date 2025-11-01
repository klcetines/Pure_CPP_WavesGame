#include "Effects/ProjectileEffects/EffectsArrange.h"

void EffectsArrange::addEffect(std::unique_ptr<IProjectileEffect> effect){
    if (effect) {
        _effects.push_back(std::move(effect));
    }
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
        if (_effects[currentEffectIndex]) {
            ProjectileAction action = _effects[currentEffectIndex]->OnImpact(enemy);
            if (action == ProjectileAction::Trigger) {
                nextEffect(); 
                if(currentEffectIndex < _effects.size()) return ProjectileAction::Destroy;
                return ProjectileAction::Continue;
            }
            return action;
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

