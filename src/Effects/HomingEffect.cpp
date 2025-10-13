#include "Effects/HomingEffect.h"

EffectType HomingEffect::GetType() const {
    return EffectType::Homing; 
}

std::unique_ptr<IProjectileEffect> HomingEffect::Clone() const {
    return std::make_unique<HomingEffect>(*this);
}