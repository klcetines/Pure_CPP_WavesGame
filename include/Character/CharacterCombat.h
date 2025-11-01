#ifndef CHARACTER_COMBAT_H
#define CHARACTER_COMBAT_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Projectiles/PlayerProjectile.h"
#include "Character/CharacterStats.h"

class CharacterCombat {
public:
    CharacterCombat(CharacterStats& stats);
    
    shared_ptr<vector<shared_ptr<PlayerProjectile>>> 
    createAttack(const Vector2f& origin, const Vector2f& target);
    
    void updateCooldowns(float dt);
    bool canShoot() const;
    float getFacingAngle() const;

private:
    static constexpr float PI = 3.14159265f;
    static constexpr float BULLET_SPACING = 15.0f;

    CharacterStats& stats;
    float shootCooldown;
    float facingAngle;

    Vector2f calculateDirection(const Vector2f& origin, const Vector2f& target) const;
    float calculateFacingAngle(const Vector2f& direction) const;
    Vector2f calculateBulletSpawnPosition(const Vector2f& origin, 
                                            const Vector2f& direction, 
                                            int bulletIndex) const;
};

#endif // CHARACTER_COMBAT_H