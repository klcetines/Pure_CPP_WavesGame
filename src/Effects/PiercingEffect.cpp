#include "Effects/PiercingEffect.h"

void PiercingEffect::OnImpact(Projectile& projectile, Enemy& enemy) {
    std::cout << "PiercingEffect: Projectile pierced through enemy " << enemy.getName() << std::endl;
}

std::unique_ptr<IProjectileEffect> PiercingEffect::Clone() const {
    return std::make_unique<PiercingEffect>(*this);
}