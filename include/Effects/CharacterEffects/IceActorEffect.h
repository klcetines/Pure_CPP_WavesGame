#ifndef ICE_ACTOR_EFFECT_H
#define ICE_ACTOR_EFFECT_H

#include "Utils/IActor.h"
#include "Effects/CharacterEffects/IActorEffect.h"

class IceActorEffect : public IActorEffect {
public:
    void onApply(IActor& character) override;
    void addCharges(int charges) override;
    bool isFinished() override;
    void onRemove(IActor& character) override;
    void update(IActor& character, float deltaTime) override;
    void refreshDuration() override;
    ActorEffectType GetType() const override;
    
    float getTickProgress() const override;
    int getCharges() const override;

private:    
    float duration = 5.0f;
    float timeSinceApply = 0.0f;
    int charges = 0;

    bool isFreezing = false;
    float freezeDuration = 3.0f;
    float freezeTimer = 0.0f;
};

#endif // ICE_ACTOR_EFFECT_H