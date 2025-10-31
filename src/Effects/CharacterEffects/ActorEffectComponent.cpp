#include "Effects/CharacterEffects/ActorEffectComponent.h"

ActorEffectComponent::ActorEffectComponent(IActor* owner) 
    : _owner(owner) {}

void ActorEffectComponent::Update(float dt) {
        auto it = _activeEffects.begin();
        while (it != _activeEffects.end()) {
            
            it->second->update(*_owner, dt);

            if (it->second->isFinished()) {
                it->second->onRemove(*_owner);
                it = _activeEffects.erase(it);
            } 
            else {
                it++;
            }
        }
    }

    void ActorEffectComponent::AddEffect(std::unique_ptr<IActorEffect> newEffect) {
        ActorEffectType type = newEffect->GetType();

        auto it = _activeEffects.find(type);

        if (it != _activeEffects.end()) {
            it->second->refreshDuration();
            it->second->addCharges(1);
        } else {
            newEffect->onApply(*_owner);
            _activeEffects[type] = std::move(newEffect);
        }
    }
    
    void ActorEffectComponent::RemoveEffect(ActorEffectType type) {
        auto it = _activeEffects.find(type);
        if (it != _activeEffects.end()) {
            it->second->onRemove(*_owner);
            _activeEffects.erase(it);
        }
    }