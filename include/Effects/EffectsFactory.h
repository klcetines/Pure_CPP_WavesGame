#ifndef EFFECTS_FACTORY_H
#define EFFECTS_FACTORY_H

#include <iostream>
#include <unordered_map>
#include <functional>
#include <memory>
#include "Effects/IProjectileEffect.h"
#include "Effects/PiercingEffect.h"
#include "Effects/HomingEffect.h"

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