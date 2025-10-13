#include "Effects/PiercingEffect.h"

ProjectileAction PiercingEffect::OnImpact(Enemy& enemy) {
    std::cout << "PiercingEffect: Projectile pierced through enemy " << enemy.getName() << std::endl;
    return ProjectileAction::Continue;
}

std::unique_ptr<IProjectileEffect> PiercingEffect::Clone() const {
    return std::make_unique<PiercingEffect>(*this);
}