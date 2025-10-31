#ifndef ACTOR_EFFECTS_FACTORY_H
#define ACTOR_EFFECTS_FACTORY_H

#include <iostream>
#include <unordered_map>
#include <functional>
#include <memory>
#include "Effects/CharacterEffects/IActorEffect.h"
#include "Effects/CharacterEffects/FireActorEffect.h"

class ActorEffectsFactory {
public:
    using CreatorFunc = std::function<std::unique_ptr<IActorEffect>()>;

    static ActorEffectsFactory& Instance();

    static void Initialize();

    void RegisterEffect(const int id, CreatorFunc func);

    std::unique_ptr<IActorEffect> Create(const int id);

private:
    std::unordered_map<int, CreatorFunc> creators;
};


#endif  // ACTOR_EFFECTS_FACTORY_H