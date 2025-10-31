#include "Effects/CharacterEffects/FireActorEffect.h"

void FireActorEffect::onApply(class IActor& character) {
    charges++;
    refreshDuration();
    if(timeUntilDamage < 0.0f) {
        timeUntilDamage = damageInterval;
    }
}

void FireActorEffect::addCharges(int additionalCharges) {
    charges += additionalCharges;
}

bool FireActorEffect::isFinished() {
    return timeSinceApply >= duration;
}

void FireActorEffect::onRemove(IActor& character){
    timeUntilDamage = -1.0f;
    charges = 0;
}

void FireActorEffect::update(IActor& character, float deltaTime){
    timeSinceApply += deltaTime;

    if (timeUntilDamage > 0.0f) {
        timeUntilDamage -= deltaTime;
    }

    if (timeUntilDamage <= 0.0f) {
        float damage = damagePerEffect * charges;
        character.takeDamage(damage);
        timeUntilDamage = damageInterval;
    }
}

void FireActorEffect::refreshDuration(){
    timeSinceApply = 0.0f;
}

ActorEffectType FireActorEffect::GetType() const {
    return ActorEffectType::Fire;
}