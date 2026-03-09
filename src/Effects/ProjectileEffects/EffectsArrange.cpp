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
    if (!effect) return;
    if(effect->extraImpact()){
        maxImpacts++;
        _impacts.reserve(maxImpacts);
    }
    _modifiers.push_back(std::move(effect));
}

const std::vector<std::unique_ptr<IProjectileEffect>>& EffectsArrange:: getModifiers() const{
    return _modifiers;
}

void EffectsArrange::addImpact(std::unique_ptr<IProjectileEffect> effect){
    if (!effect) return;
    _impacts.push_back(std::move(effect));
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

void EffectsArrange::nextEffect(Projectile& projectile){
    if (!_modifiers.empty() && currentEffectIndex < _modifiers.size()) {
        currentEffectIndex++;
        
        int i = currentEffectIndex;
        bool passive = true;
        
        while(i < _modifiers.size() && passive) {
            _modifiers[i]->OnFire(projectile);
            if (!_modifiers[i]->isPassive()) {
                passive = false;
            }
            i++;
        }
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

std::unique_ptr<EffectsArrange> EffectsArrange::CloneFromIndex(int index) const {
    auto clone = std::make_unique<EffectsArrange>();
    
    for (int i = index; i < _modifiers.size(); ++i) {
        clone->addModifier(_modifiers[i]->Clone());
    }
    
    for (const auto& impact : _impacts) {
        clone->addImpact(impact->Clone());
    }
    return clone;
}

EffectType EffectsArrange::GetType() const {
    if (!_modifiers.empty() && currentEffectIndex < _modifiers.size()) {
        return _modifiers[currentEffectIndex]->GetType();
    }
    else return EffectType::Generic; 
}

bool EffectsArrange::hasActiveEffect(EffectType type) const {
    if (_modifiers.empty() || currentEffectIndex >= _modifiers.size()) return false;
    
    bool hasPassive = false;
    bool hasActive = false;
    int i = currentEffectIndex;
    
    while (i < _modifiers.size()) {
        bool isPassive = _modifiers[i]->isPassive();
        
        if (isPassive) {
            if (hasPassive) break; 
            hasPassive = true;
        } else {
            if (hasActive) break; 
            hasActive = true;
        }
        
        if (_modifiers[i]->GetType() == type) {
            return true; 
        }
        i++;
    }
    return false;
}

void EffectsArrange::OnFire(Projectile& projectile){
    if (_modifiers.empty() || currentEffectIndex >= _modifiers.size()) return;

    int i = currentEffectIndex;
    bool passive = true;
    
    while(i < _modifiers.size() && passive) {
        _modifiers[i]->OnFire(projectile);
        if (!_modifiers[i]->isPassive()) {
            passive = false;
        }
        i++;
    }
}

void EffectsArrange::OnUpdate(Projectile& projectile, float deltaTime) {
    if (_modifiers.empty() || currentEffectIndex >= _modifiers.size()) return;

    bool hasPassive = false;
    bool hasActive = false;
    int i = currentEffectIndex;
    
    while(i < _modifiers.size()) {
        bool isPassive = _modifiers[i]->isPassive();
        if (isPassive) {
            if (hasPassive) break;
            hasPassive = true;
        } 
        else {
            if (hasActive) break;
            hasActive = true;
        }

        if (_modifiers[i]->OnUpdate(projectile, deltaTime, i) == ProjectileAction::Trigger) {
            triggerSpecificEffect(_modifiers[i]->GetType(), projectile);
            break; 
        }
        i++;
    }
}

ProjectileAction EffectsArrange::OnImpact(Projectile& projectile, Enemy& enemy) {
    if (currentImpactIndex < _impacts.size()) {
        _impacts[currentImpactIndex]->OnImpact(enemy);
        currentImpactIndex++; 
    }

    if (_modifiers.empty() || currentEffectIndex >= _modifiers.size()) {
        return ProjectileAction::Destroy;
    }

    bool hasPassive = false;
    bool hasActive = false;
    int i = currentEffectIndex;
    ProjectileAction finalAction = ProjectileAction::Continue;
    
    bool triggered = false;
    EffectType triggerType;

    while(i < _modifiers.size()) {
        bool isPassive = _modifiers[i]->isPassive();
        if (isPassive) {
            if (hasPassive) break;
            hasPassive = true;
        } else {
            if (hasActive) break;
            hasActive = true;
        }

        ProjectileAction action = _modifiers[i]->OnImpact(enemy);
        
        if (action == ProjectileAction::Trigger) {
            triggered = true;
            triggerType = _modifiers[i]->GetType(); // Guardamos quién hizo el trigger
        } else if (action == ProjectileAction::Destroy) {
            finalAction = ProjectileAction::Destroy;
        }
        i++;
    }

    if (triggered) {
        triggerSpecificEffect(triggerType, projectile);
        if (currentEffectIndex >= _modifiers.size()) {
            return ProjectileAction::Destroy;
        }
        return ProjectileAction::Continue; 
    }
        
    return finalAction;
}

void EffectsArrange::OnDistanceTraveled(Projectile& projectile, float distance){
    if (!_modifiers.empty() && currentEffectIndex < _modifiers.size()) {
       if (_modifiers[currentEffectIndex]->OnDistanceTraveled(projectile, distance ) == ProjectileAction::Trigger) {
            nextEffect(projectile);
        }
    }
}

void EffectsArrange::OnExpire(Projectile& projectile){
    if (!_modifiers.empty() && currentEffectIndex < _modifiers.size()) {
        if(_modifiers[currentEffectIndex]->OnExpire(projectile) == ProjectileAction::Trigger) {
            nextEffect(projectile);
        }
    }
}

void EffectsArrange::swapEffects(int index1, int index2) {
    int totalEffects = maxModifiers + _impacts.size();

    if (index1 < 0 || index1 >= totalEffects || index2 < 0 || index2 >= totalEffects) {
        return; 
    }

    std::unique_ptr<IProjectileEffect>* effect1;
    std::unique_ptr<IProjectileEffect>* effect2;

    if (index1 < _modifiers.size()) {
        effect1 = &_modifiers[index1];
    } 
    else {
        effect1 = &_impacts[index1 - maxModifiers];
    }

    if (index2 < _modifiers.size()) {
        effect2 = &_modifiers[index2];
    } 
    else {
        effect2 = &_impacts[index2 - maxModifiers];
    }
    std::swap(*effect1, *effect2);
}

void EffectsArrange::triggerSpecificEffect(EffectType type, Projectile& projectile) {
    if (_modifiers.empty() || currentEffectIndex >= _modifiers.size()) return;

    bool hasPassive = false;
    bool hasActive = false;
    
    for (size_t i = currentEffectIndex; i < _modifiers.size(); ++i) {
        bool isPassive = _modifiers[i]->isPassive();
        if (isPassive) {
            if (hasPassive) break;
            hasPassive = true;
        } else {
            if (hasActive) break;
            hasActive = true;
        }

        if (_modifiers[i]->GetType() == type) {
            currentEffectIndex = i;
            nextEffect(projectile);
            break;
        }
    }
}

