#ifndef PLAYER_PROJECTILE_H
#define PLAYER_PROJECTILE_H

#include "Projectiles/Projectile.h"
#include "Utils/CollisionShape.h"
#include "Enemies/Enemy.h"
#include "Effects/ProjectileEffects/EffectsArrange.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <memory>
#include <unordered_set>

class PlayerProjectile : public Projectile {
public:
    PlayerProjectile(Vector2f start, Vector2f direction, float speed, float damage, const EffectsArrange& effects, float range = 300.0f);
    ~PlayerProjectile();

    void update(float dt, shared_ptr<Enemy> closestEnemy = nullptr);

    void draw(RenderWindow& window, float offsetX, float offsetY) override;
    void handleImpact(IActor& actor) override;
    void destroy() override;
    CollisionShape getCollisionBox() const override;
    
private:
    float _maxLifetime = 5.0f;
    float _damage;
    
    CircleShape _shape;
    CollisionShape _collisionBox;

    unordered_set<int> _hitEnemies;
    Enemy* _lastHitEnemy = nullptr;

    void handleHomingLogic(float dt, shared_ptr<Enemy> target);
    void handlePiercingLogic();
    
    float getVectorLength(const Vector2f& v);
    Vector2f normalizeVector(const Vector2f& v);
};

#endif