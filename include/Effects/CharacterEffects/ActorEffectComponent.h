#ifndef ACTOR_EFFECT_COMPONENT_H
#define ACTOR_EFFECT_COMPONENT_H

#include <iostream>
#include <map>
#include <memory>
#include "Effects/CharacterEffects/ActorEffectComponent.h"
#include "Effects/CharacterEffects/IActorEffect.h"

class IActor;

class ActorEffectComponent {
public:
    ActorEffectComponent() = default;
    ActorEffectComponent(IActor* owner);

    ~ActorEffectComponent() = default;

    void Update(float dt);
    void UpdateDistance(float distance);
    void AddEffect(std::unique_ptr<IActorEffect> newEffect);
    void RemoveEffect(ActorEffectType type);
    bool ItsEmpty() const;
    bool find(ActorEffectType type) const;
    IActorEffect* get(ActorEffectType type); 

private:
    IActor* _owner; 
    
    std::map<ActorEffectType, std::unique_ptr<IActorEffect>> _activeEffects;

};

#endif // ACTOR_EFFECT_COMPONENT_H