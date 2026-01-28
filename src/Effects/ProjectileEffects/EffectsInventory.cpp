#include "Effects/ProjectileEffects/EffectsInventory.h"

EffectsInventory::EffectsInventory(){
    maxInventorySize = 10;
    _unusedEffects.reserve(maxInventorySize);
}

EffectsInventory::~EffectsInventory(){
    _unusedEffects.clear();
}

bool EffectsInventory::isFull() const{
    return _unusedEffects.size() >= maxInventorySize;
}

void EffectsInventory::addEffect(const IProjectileEffect& effect){
    if (_unusedEffects.size() < maxInventorySize) {
        _unusedEffects.push_back(unique_ptr<IProjectileEffect>(effect.Clone()));
    }
}