#ifndef I_ANIMATED_OBJECT__EFFECT
#define I_ANIMATED_OBJECT__EFFECT

enum class AOEffectType {
    Fire,
    Poison,
    Blessing,
    StatBuff
};

class IAnimatedObjectEffect {
    
    public:
        virtual ~IAnimatedObjectEffect() = default;

        virtual void onApply(class IAnimatedObject& character) = 0;
        virtual bool isFinished() = 0;
        virtual void onRemove(class IAnimatedObject& character) = 0;
        virtual void update(class IAnimatedObject& character, float deltaTime) = 0;
        virtual void refreshDuration() = 0;
        virtual AOEffectType GetType() const = 0;
};

#endif // I_ANIMATED_OBJECT__EFFECT