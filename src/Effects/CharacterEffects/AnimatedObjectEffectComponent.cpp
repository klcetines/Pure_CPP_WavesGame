#include "Effects/CharacterEffects/AnimatedObjectEffectComponent.h"

AnimatedObjectEffectComponent::AnimatedObjectEffectComponent(IAnimatedObject* owner) 
    : _owner(owner) {}

void AnimatedObjectEffectComponent::Update(float dt) {
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

    void AnimatedObjectEffectComponent::AddEffect(std::unique_ptr<IAnimatedObjectEffect> newEffect) {
        AOEffectType type = newEffect->GetType();

        auto it = _activeEffects.find(type);

        if (it != _activeEffects.end()) {
            it->second->refreshDuration(); 
            // it->second->AddStacks(1);
        } else {
            newEffect->onApply(*_owner);
            _activeEffects[type] = std::move(newEffect);
        }
    }
    
    void AnimatedObjectEffectComponent::RemoveEffect(AOEffectType type) {
        auto it = _activeEffects.find(type);
        if (it != _activeEffects.end()) {
            it->second->onRemove(*_owner);
            _activeEffects.erase(it);
        }
    }