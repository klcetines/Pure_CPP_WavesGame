#include "Character/CharacterCombat.h"

CharacterCombat::CharacterCombat(CharacterStats& stats)
    : stats(stats)
    , shootCooldown(0.0f)
    , facingAngle(0.0f)
{}

shared_ptr<vector<shared_ptr<PlayerProjectile>>> 
CharacterCombat::createAttack(const Vector2f& origin, const Vector2f& target) {
    auto projectiles = make_shared<vector<shared_ptr<PlayerProjectile>>>();
    
    if (!canShoot()) {
        return projectiles;
    }

    Vector2f direction = calculateDirection(origin, target);
    facingAngle = calculateFacingAngle(direction);
    
    for (int i = 0; i < stats.getBulletsNumber(); i++) {
        Vector2f spawnPos = calculateBulletSpawnPosition(origin, direction, i);
        projectiles->push_back(
            make_shared<PlayerProjectile>(
                spawnPos, 
                target, 
                stats.getProjectileSpeed(),
                stats.getDamage(),
                stats.getProjectileEffects()
            )
        );
    }

    shootCooldown = stats.getAttackSpeed();
    return projectiles;
}

void CharacterCombat::updateCooldowns(float dt) {
    if (shootCooldown > 0) {
        shootCooldown = max(0.0f, shootCooldown - dt);
    }
}

bool CharacterCombat::canShoot() const {
    return shootCooldown <= 0;
}

float CharacterCombat::getFacingAngle() const {
    return facingAngle;
}

Vector2f CharacterCombat::calculateDirection(const Vector2f& origin, const Vector2f& target) const {
    Vector2f dir = target - origin;
    float length = sqrt(dir.x * dir.x + dir.y * dir.y);
    return length > 0 ? dir / length : Vector2f(1, 0);
}

float CharacterCombat::calculateFacingAngle(const Vector2f& direction) const {
    return atan2(direction.y, direction.x) * 180.0f / PI - 90.0f;
}

Vector2f CharacterCombat::calculateBulletSpawnPosition(const Vector2f& origin, 
                                                     const Vector2f& direction, 
                                                     int bulletIndex) const {
    Vector2f offset = direction * (bulletIndex * BULLET_SPACING);
    return origin + offset;
}