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

    void setSpawnCallback(SpawnCallback callback) override;
    void update(float dt, shared_ptr<Enemy> closestEnemy = nullptr);

    void draw(RenderWindow& window, float offsetX, float offsetY) override;
    void handleImpact(std::shared_ptr<IActor> actor) override;
    void destroy() override;
    CollisionShape getCollisionBox() const override;
    float getDamage() const override;
    void updateCollisionBox() override;
    void createShapeFromEffects();
    
private:
    float _maxLifetime = 5.0f;
    float _damage;
    
    std::unique_ptr<sf::Shape> _shape;
    CollisionShape _collisionBox;

    unordered_set<int> _hitEnemies;
    std::weak_ptr<Enemy> _lastHitEnemy;
    bool _effectApplied = false;

    void handleHomingLogic(float dt, shared_ptr<Enemy> target);
    void handlePiercingLogic();
    
    float getVectorLength(const Vector2f& v);
    Vector2f normalizeVector(const Vector2f& v);
    void processImpactEffects(Enemy& enemy);
};

#endif