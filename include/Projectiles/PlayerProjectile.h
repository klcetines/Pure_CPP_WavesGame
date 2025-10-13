#ifndef PLAYER_PROJECTILE_H
#define PLAYER_PROJECTILE_H

#include "Projectiles/Projectile.h"
#include "Utils/CollisionShape.h"
#include "Enemies/Enemy.h"
#include "Effects/EffectsArrange.h"
#include <SFML/Graphics.hpp>
#include <cmath>

class PlayerProjectile : public Projectile {
public:
    PlayerProjectile(sf::Vector2f start, sf::Vector2f target, float speed, float damage, const EffectsArrange& effects, float range = 300.0f);
    
    void update (float dt) override;
    void update(float dt, shared_ptr<Enemy> closest_enemy);
    
    void addEffect(std::unique_ptr<IProjectileEffect> effect);

    void draw(RenderWindow& window, float offsetX, float offsetY) override;
    bool isAlive() const override;

    void handleImpact(Enemy& enemy) override;

    sf::Vector2f getPosition() const override;
    float getDamage() const override;
    float getSize() const override;
    void destroy() override;
    CollisionShape getCollisionBox() const override;
    
private:
    CircleShape shape;
    Vector2f position;

    Vector2f velocity;
    bool _alive;
    float _lifetime = 0.f;
    float _damage;
    float _maxRange;
    float _traveledDistance = 0.f;
    unique_ptr<EffectsArrange> _effects;

    int currentEffectIndex = 0;

    CollisionShape _collisionBox;
    void updateDistanceTraveled(const Vector2f& movement);
    void updatePosition(const Vector2f& movement);
    void updateCollisionBox();
    void updateProjectileEffects();
    void updateVelocityTowardsTarget(const Vector2f& targetDirection, float deltaTime);
};

#endif