#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <functional>
#include <SFML/Graphics.hpp>
#include "Enemies/Enemy.h"
#include "Utils/CollisionShape.h"

class EffectsArrange;

using namespace sf;

class Projectile {
public:
    Projectile();
    virtual ~Projectile() = default;
    
    virtual void update(float dt);
    
    virtual void draw(RenderWindow& window, float offsetX, float offsetY) = 0;
    virtual bool isAlive() const;
    virtual void destroy() = 0;

    virtual void handleImpact(std::shared_ptr<IActor> enemy);

    virtual float getDamage() const;
    virtual float getSize() const;
    virtual Vector2f getPosition() const;
    virtual Vector2f getVelocity() const;
    virtual CollisionShape getCollisionBox() const;
    virtual float getTraveledDistance() const;
    virtual float getMaxRange() const;
    virtual const EffectsArrange& getEffects() const;

    using SpawnCallback = std::function<void(std::shared_ptr<Projectile>)>;
    virtual void setSpawnCallback(SpawnCallback callback);
    virtual void spawnChild(std::shared_ptr<Projectile> child);

protected:
    Vector2f _position;
    Vector2f _velocity;
    unique_ptr<EffectsArrange> _effects;
    float _lifetime = 0.0f;
    
    float _traveledDistance = 0.0f;
    float _maxRange = 1000.0f;
    bool _alive = true;

    SpawnCallback _spawnCallback;
    
    virtual void updatePosition(const Vector2f& movement);
    virtual void updateDistanceTraveled(const Vector2f& movement);
    virtual void updateCollisionBox();
};

#endif // PROJECTILE_H