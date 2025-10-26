#include "Effects/ProjectileEffects/PiercingEffect.h"

ProjectileAction PiercingEffect::OnImpact(Enemy& enemy) {
    return ProjectileAction::Continue;
}

std::unique_ptr<IProjectileEffect> PiercingEffect::Clone() const {
    return std::make_unique<PiercingEffect>(*this);
}