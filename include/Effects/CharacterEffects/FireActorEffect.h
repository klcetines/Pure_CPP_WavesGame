#ifndef FIRE_ACTOR_EFFECT_H
#define FIRE_ACTOR_EFFECT_H

#include "Utils/IActor.h"
#include "Effects/CharacterEffects/IActorEffect.h"

class FireActorEffect : public IActorEffect {
public:
    void onApply(IActor& character) override;
    void addCharges(int charges) override;
    bool isFinished() override;
    void onRemove(IActor& character) override;
    void update(IActor& character, float deltaTime) override;
    void refreshDuration() override;
    ActorEffectType GetType() const override;
private:    
    float duration = 3.0f;
    float timeSinceApply = 0.0f;

    float timeUntilDamage = -1.0f;
    float damageInterval = 1.0f;

    float damagePerEffect = 10.0f;
    int charges = 0;

};

#endif // FIRE_ACTOR_EFFECT_H