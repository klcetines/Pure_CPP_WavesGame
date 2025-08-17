// Projectile.h
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include "Utils/CollisionShape.h"

using namespace sf;

class Projectile {
public:
    virtual ~Projectile() = default;
    virtual void update(float dt) = 0;
    virtual void draw(RenderWindow& window, float offsetX, float offsetY) = 0;
    virtual bool isAlive() const = 0;
    virtual Vector2f getPosition() const = 0;
    virtual float getDamage() const = 0;
    virtual float getSize() const = 0;
    virtual void destroy() = 0;
    virtual CollisionShape getCollisionBox() const = 0;
};

#endif