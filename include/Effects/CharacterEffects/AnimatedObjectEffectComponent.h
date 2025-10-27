#ifndef ANIMATED_OBJECT_EFFECT_COMPONENT_H
#define ANIMATED_OBJECT_EFFECT_COMPONENT_H

#include <map>
#include <memory>
#include "Effects/CharacterEffects/IAnimatedObjectEffect.h"
#include "Utils/IAnimatedObject.h"

class AnimatedObjectEffectComponent {
public:
    AnimatedObjectEffectComponent(IAnimatedObject* owner);

    void Update(float dt);

    void AddEffect(std::unique_ptr<IAnimatedObjectEffect> newEffect);
    
    void RemoveEffect(AOEffectType type);

private:
    IAnimatedObject* _owner; 
    
    std::map<AOEffectType, std::unique_ptr<IAnimatedObjectEffect>> _activeEffects;

};

#endif // ANIMATED_OBJECT_EFFECT_COMPONENT_H