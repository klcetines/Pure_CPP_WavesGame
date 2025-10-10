#include "Effects/EffectsFactory.h"

EffectFactory& EffectFactory::Instance() {
    static EffectFactory instance;
    return instance;
}

void EffectFactory::Initialize(){
    Instance().RegisterEffect(0, []() { return std::make_unique<PiercingEffect>(); });
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