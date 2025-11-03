#include "Effects/ProjectileEffects/EffectsFactory.h"

EffectFactory& EffectFactory::Instance() {
    static EffectFactory instance;
    return instance;
}

void EffectFactory::Initialize(){
    Instance().RegisterEffect(0, []() { return std::make_unique<PiercingEffect>(); });
    Instance().RegisterEffect(1, []() { return std::make_unique<HomingEffect>(); });
    Instance().RegisterEffect(2, []() { return std::make_unique<FireProjectileEffect>(); });
    Instance().RegisterEffect(3, []() { return std::make_unique<BleedProjectileEffect>(); });
}

void EffectFactory::RegisterEffect(const int id, CreatorFunc func) {
    creators[id] = func;
}

std::unique_ptr<IProjectileEffect> EffectFactory::Create(const int id) {
    auto it = creators.find(id);
    if (it != creators.end()) {
        return it->second();
    }
    else{
        std::cout << "EffectFactory: Unknown effect id '" << id << "'" << std::endl;
    }
    return nullptr;
}