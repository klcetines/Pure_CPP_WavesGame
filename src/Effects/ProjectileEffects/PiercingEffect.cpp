#include "Effects/ProjectileEffects/PiercingEffect.h"

EffectType PiercingEffect::GetType() const {
    return EffectType::Piercing; 
}

ProjectileAction PiercingEffect::OnImpact(IActor& enemy) {
    return ProjectileAction::Continue;
}

std::unique_ptr<IProjectileEffect> PiercingEffect::Clone() const {
    return std::make_unique<PiercingEffect>(*this);
}