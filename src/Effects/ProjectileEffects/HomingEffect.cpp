#include "Effects/ProjectileEffects/HomingEffect.h"

EffectType HomingEffect::GetType() const {
    return EffectType::Homing; 
}

ProjectileAction HomingEffect::OnImpact(Enemy& enemy) {
    return ProjectileAction::Trigger;
}

std::unique_ptr<IProjectileEffect> HomingEffect::Clone() const {
    return std::make_unique<HomingEffect>(*this);
}