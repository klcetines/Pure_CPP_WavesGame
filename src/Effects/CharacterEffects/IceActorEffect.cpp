#include "Effects/CharacterEffects/IceActorEffect.h"
#include <algorithm>

void IceActorEffect::onApply(IActor& character) {
    charges++;
    refreshDuration();
}

void IceActorEffect::addCharges(int additionalCharges) {
    charges += additionalCharges;
    refreshDuration();
}

bool IceActorEffect::isFinished() {
    return timeSinceApply >= duration;
}

void IceActorEffect::onRemove(IActor& character){
    if (isFreezing) {
        character.setFrozen(false);
    }
    charges = 0;
    isFreezing = false;
}

void IceActorEffect::update(IActor& character, float deltaTime){
    timeSinceApply += deltaTime;

    if (charges >= 3 && !isFreezing) {
        isFreezing = true;
        character.setFrozen(true);
        freezeTimer = freezeDuration; 
        
    }

    if (isFreezing) {
        freezeTimer -= deltaTime;
        if (freezeTimer <= 0.0f) {
            character.setFrozen(false);
            isFreezing = false;
            charges = 0;
            
            timeSinceApply = duration; 
        }
    }
}

void IceActorEffect::refreshDuration(){
    timeSinceApply = 0.0f;
}

ActorEffectType IceActorEffect::GetType() const {
    return ActorEffectType::Ice;
}

float IceActorEffect::getTickProgress() const {
    if (!isFreezing) return 0.0f;
    return std::max(0.0f, freezeTimer) / freezeDuration; 
}

int IceActorEffect::getCharges() const {
    return charges;
}