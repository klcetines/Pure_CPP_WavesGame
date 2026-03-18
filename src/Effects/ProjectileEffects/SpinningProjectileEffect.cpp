#include "Effects/ProjectileEffects/SpinningProjectileEffect.h"
#include "Projectiles/PlayerProjectile.h"

EffectType SpinningProjectileEffect::GetType() const{
    return EffectType::Generic;
}

std::string SpinningProjectileEffect::getModifierSymbol() const{
    return "S";
}

uint32_t SpinningProjectileEffect::getColorCode() const{
    return 0xFFDDDDDD;
}

ProjectileAction SpinningProjectileEffect::OnFire(Projectile& projectile){
    _shotsFired = 0;
    _degreesSpin = 0.0f;
    _lastDistance = projectile.getTraveledDistance();
    _distUntilShot = projectile.getMaxRange() / 10.0f; 
    _distBetweenShot = (projectile.getMaxRange() - (_distUntilShot * 2)) / 7.0f;
    
    return ProjectileAction::Continue;
}

ProjectileAction SpinningProjectileEffect::OnUpdate(Projectile& projectile, float deltaTime, int myIndex) {
    ProjectileAction action = ProjectileAction::Continue;
    float currentDist = projectile.getTraveledDistance();
    float distanceCovered = currentDist - _lastDistance;
    _lastDistance = currentDist;

    _distUntilShot -= distanceCovered;
    if (_distUntilShot <= 0) {
        Vector2f baseDir = projectile.getVelocity();
        float radians = _degreesSpin * (M_PI / 180.0f);
        Vector2f newDir;
        newDir.x = baseDir.x * cos(radians) - baseDir.y * sin(radians);
        newDir.y = baseDir.x * sin(radians) + baseDir.y * cos(radians);

        auto* playerProj = dynamic_cast<PlayerProjectile*>(&projectile);
        if (playerProj) {
            auto nextEffects = playerProj->getEffects().CloneFromIndex(myIndex + 1);        
            if (nextEffects) {
                auto childProj = std::make_shared<PlayerProjectile>(
                    projectile.getPosition(),
                    newDir,
                    100.0f,
                    projectile.getDamage() / 3.0f,
                    *nextEffects,
                    projectile.getMaxRange() 
                );
                projectile.spawnChild(childProj);
            }
        }
        _shotsFired++;
        _degreesSpin += 45.0f;
        _distUntilShot = _distBetweenShot;
        
        if (_shotsFired >= 7) {
            action = ProjectileAction::Trigger;
        }
        
    }
    return action;
}

ProjectileAction SpinningProjectileEffect::OnImpact(IActor& enemy){
    return ProjectileAction::Trigger;
}

std::unique_ptr<IProjectileEffect> SpinningProjectileEffect::Clone() const{
    return std::make_unique<SpinningProjectileEffect>(*this);
}

bool SpinningProjectileEffect::extraImpact() const {
    return false;
}
