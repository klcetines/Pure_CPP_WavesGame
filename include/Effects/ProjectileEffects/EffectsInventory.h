#ifndef EFFECTS_INVENTORY_H
#define EFFECTS_INVENTORY_H

#include <vector>
#include <memory>
#include "Effects/ProjectileEffects/IProjectileEffect.h"

class EffectsInventory {
public:
    EffectsInventory();
    ~EffectsInventory();
    bool isFull() const;
    void addEffect(const IProjectileEffect& effect);
    void removeEffect(const IProjectileEffect& effect);
    std::vector<std::unique_ptr<IProjectileEffect>>& getEffects();
    const std::vector<std::unique_ptr<IProjectileEffect>>& getEffects() const;
    int getMaxInventorySize() const;

private:
    std::vector<std::unique_ptr<IProjectileEffect>> _unusedEffects;
    int maxInventorySize;
};

#endif // EFFECTS_INVENTORY_H