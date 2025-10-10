#include "Effects/HomingEffect.h"

EffectType HomingEffect::GetType() const {
    return EffectType::Homing; 
}

void HomingEffect::OnImpact(Projectile& projectile, Enemy& enemy) {
    projectile.destroy();
}

std::unique_ptr<IProjectileEffect> HomingEffect::Clone() const {
    return std::make_unique<HomingEffect>(*this);
}