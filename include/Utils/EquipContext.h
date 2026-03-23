#pragma once
#include "Effects/ProjectileEffects/EffectsArrange.h"
#include "Effects/ProjectileEffects/EffectsInventory.h"

struct EquipContext {
    EffectsArrange*   arrange   = nullptr;
    EffectsInventory* inventory = nullptr;

    bool isValid() const { return arrange != nullptr && inventory != nullptr; }
};