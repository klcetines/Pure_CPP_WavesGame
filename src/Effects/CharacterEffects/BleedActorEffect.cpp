#include "Effects/CharacterEffects/BleedActorEffect.h"

void BleedActorEffect::onApply(class IActor& character) {
    damageInterval = 100.0f;
    charges++;
    refreshDuration();
    if(distanceUntilDamage < 0.0f) {
        distanceUntilDamage = damageInterval;
    }
}

void BleedActorEffect::addCharges(int additionalCharges) {
    charges += additionalCharges;
}

bool BleedActorEffect::isFinished() {
    return timeSinceApply >= duration;
}

void BleedActorEffect::onRemove(IActor& character){
    distanceUntilDamage = -1.0f;
    charges = 0;
}

void BleedActorEffect::update(IActor& character, float deltaTime){
    timeSinceApply += deltaTime;
    if(timeSinceApply > duration){
        timeSinceApply = duration;
    }
}

void BleedActorEffect::updateOnDistance(IActor& character, float distance){
    if (distanceUntilDamage > 0.0f) {
        distanceUntilDamage -= distance;
    }
    if (distanceUntilDamage <= 0.0f) {
        float damage = damagePerEffect * charges * character.getSpeed();
        character.takeDamage(damage);
        distanceUntilDamage = damageInterval;
    }
}

void BleedActorEffect::refreshDuration(){
    timeSinceApply = 0.0f;
}

ActorEffectType BleedActorEffect::GetType() const {
    return ActorEffectType::Bleed;
}