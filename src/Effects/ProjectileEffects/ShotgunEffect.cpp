#include "Effects/ProjectileEffects/ShotgunEffect.h"
#include "Projectiles/PlayerProjectile.h"

EffectType ShotgunEffect::GetType() const{
    return EffectType::Generic;
}

std::string ShotgunEffect::getModifierSymbol() const{
    return "C";
}

uint32_t ShotgunEffect::getColorCode() const{
    return 0x1c0a19dd;
}

ProjectileAction ShotgunEffect::OnFire(Projectile& projectile, int myIndex){
    float _maxDistance = (projectile.getMaxRange() - projectile.getTraveledDistance()) / 2.0f;
    auto* playerProj = dynamic_cast<PlayerProjectile*>(&projectile);
    if (playerProj) {
        auto nextEffects = playerProj->getEffects().CloneFromIndex(myIndex + 1);
        if (nextEffects) {
            float childDamage = projectile.getDamage() / 3.0f;
            Vector2f baseDir = projectile.getVelocity();
            float angle = M_PI / 4.0f;
            
            sf::Vector2f dir1;
            dir1.x = baseDir.x * cos(angle) - baseDir.y * sin(angle);
            dir1.y = baseDir.x * sin(angle) + baseDir.y * cos(angle);
            
            sf::Vector2f dir2; 
            dir2.x = baseDir.x * cos(-angle) - baseDir.y * sin(-angle);
            dir2.y = baseDir.x * sin(-angle) + baseDir.y * cos(-angle);

            auto childProj1 = std::make_shared<PlayerProjectile>(
                projectile.getPosition(),
                dir1,
                100.0f,
                childDamage,
                *nextEffects,
                _maxDistance
            );
            projectile.spawnChild(childProj1);
            
            auto childProj2 = std::make_shared<PlayerProjectile>(
                projectile.getPosition(),
                dir2,
                100.0f,
                childDamage,
                *nextEffects,
                _maxDistance
            );
            projectile.spawnChild(childProj2);
        }
    }
    return ProjectileAction::Trigger;
}

std::unique_ptr<IProjectileEffect> ShotgunEffect::Clone() const {
    return std::make_unique<ShotgunEffect>(*this);
}