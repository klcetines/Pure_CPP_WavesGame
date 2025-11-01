#include "Effects/ProjectileEffects/HomingEffect.h"

EffectType HomingEffect::GetType() const {
    return EffectType::Homing; 
}

ProjectileAction HomingEffect::OnImpact(IActor& enemy) {
    return ProjectileAction::Trigger;
}

std::unique_ptr<IProjectileEffect> HomingEffect::Clone() const {
    return std::make_unique<HomingEffect>(*this);
}