#include "Effects/CharacterEffects/ActorEffectsFactory.h"

ActorEffectsFactory& ActorEffectsFactory::Instance() {
    static ActorEffectsFactory instance;
    return instance;
}

void ActorEffectsFactory::Initialize(){
    Instance().RegisterEffect(2, []() { return std::make_unique<FireActorEffect>(); });
}

void ActorEffectsFactory::RegisterEffect(const int id, CreatorFunc func) {
    creators[id] = func;
}

std::unique_ptr<IActorEffect> ActorEffectsFactory::Create(const int id) {
    auto it = creators.find(id);
    if (it != creators.end()) {
        return it->second();
    }
    else{
        std::cout << "AcotrEffectFactory: Unknown effect id '" << id << "'" << std::endl;
    }
    return nullptr;
}