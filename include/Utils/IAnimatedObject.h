#ifndef I_ANIMATED_OBJECT_H
#define I_ANIMATED_OBJECT_H

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

#include "Utils/CollisionShape.h"

using namespace sf;
using namespace std;

class IAnimatedObject {
public:
    ~IAnimatedObject() = default;
    virtual void update(float dt) = 0;
    virtual void move(float dx, float dy) = 0;
    virtual void draw(RenderWindow& window, float offsetX = 0, float offsetY = 0) = 0;
    
    virtual string getName() const = 0;
    virtual Vector2f getPosition() const = 0;
    virtual float getLife() const = 0;
    virtual CollisionShape getCollisionBox() const = 0;
    virtual float getSize() const = 0;

private:
    string name;
    Vector2f position;
    float size;
    float damageCooldown;

    CollisionShape collisionBox;
};

#endif // I_ANIMATED_OBJECT_H