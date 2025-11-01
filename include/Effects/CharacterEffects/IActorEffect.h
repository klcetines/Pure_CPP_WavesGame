#ifndef I_ACTOR_EFFECT
#define I_ACTOR_EFFECT

class IActor;

enum class ActorEffectType {
    Fire,
    Poison,
    Blessing,
    StatBuff
};

class IActorEffect {
    
    public:
        virtual ~IActorEffect() = default;

        virtual void onApply(IActor& character) = 0;
        virtual void addCharges(int charges) = 0;
        virtual bool isFinished() = 0;
        virtual void onRemove(IActor& character) = 0;
        virtual void update(IActor& character, float deltaTime) = 0;
        virtual void refreshDuration() = 0;
        virtual ActorEffectType GetType() const = 0;

    private:
        int charges = 0;
        float duration = 3.0f;
        float elapsedTime = 0.0f;

};

#endif // I_ACTOR_EFFECT