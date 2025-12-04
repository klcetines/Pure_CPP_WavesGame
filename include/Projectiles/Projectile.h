#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include "Enemies/Enemy.h"
#include "Utils/CollisionShape.h"

using namespace sf;

class Projectile {
public:
    virtual ~Projectile() = default;
    
    virtual void update(float dt);
    
    virtual void draw(RenderWindow& window, float offsetX, float offsetY) = 0;
    virtual bool isAlive() const = 0;
    virtual void destroy() = 0;

    virtual void handleImpact(IActor& enemy);

    virtual float getDamage() const;
    virtual float getSize() const;
    virtual Vector2f getPosition() const;
    virtual CollisionShape getCollisionBox() const;

protected:
    Vector2f _position;
    Vector2f _velocity;
    float _lifetime = 0.0f;
    
    float _traveledDistance = 0.0f;
    float _maxRange = 1000.0f;
    bool _alive = true;
    
    virtual void updatePosition(const Vector2f& movement);
    virtual void updateDistanceTraveled(const Vector2f& movement);
    virtual void updateCollisionBox();
};

#endif // PROJECTILE_H