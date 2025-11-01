#ifndef EFFECTS_FACTORY_H
#define EFFECTS_FACTORY_H

#include <iostream>
#include <unordered_map>
#include <functional>
#include <memory>
#include "Effects/ProjectileEffects/IProjectileEffect.h"
#include "Effects/ProjectileEffects/PiercingEffect.h"
#include "Effects/ProjectileEffects/HomingEffect.h"
#include "Effects/ProjectileEffects/FireProjectileEffect.h"

class EffectFactory {
public:
    using CreatorFunc = std::function<std::unique_ptr<IProjectileEffect>()>;

    static EffectFactory& Instance();

    static void Initialize();

    void RegisterEffect(const int id, CreatorFunc func);

    std::unique_ptr<IProjectileEffect> Create(const int id);

private:
    std::unordered_map<int, CreatorFunc> creators;
};


#endif  // EFFECTS_FACTORY_H