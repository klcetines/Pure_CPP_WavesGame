#ifndef I_ACTOR_H
#define I_ACTOR_H

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

#include "Utils/CollisionShape.h"

class ActorEffectComponent;

using namespace sf;
using namespace std;

class IActor {
public:
    ~IActor() = default;
    virtual void update(float dt) = 0;
    virtual void move(float dx, float dy) = 0;
    virtual void draw(RenderWindow& window, float offsetX = 0, float offsetY = 0) = 0;
    
    virtual string getName() const = 0;
    virtual Vector2f getPosition() const = 0;
    virtual float getLife() const = 0;
    virtual CollisionShape getCollisionBox() const = 0;
    virtual float getSize() const = 0;
    virtual void takeDamage(float damage) = 0;

    virtual ActorEffectComponent* getEffectComponent() = 0;
    virtual const ActorEffectComponent* getEffectComponent() const = 0;

private:
    string name;
    Vector2f position;
    float size;
    float damageCooldown;

    CollisionShape collisionBox;
};

#endif // I_ACTOR_H