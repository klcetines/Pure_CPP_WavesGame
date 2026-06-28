#ifndef I_ACTOR_H
#define I_ACTOR_H

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

#include "Utils/CollisionShape.h"

class ActorEffectComponent;


class IActor {
public:
    virtual ~IActor() = default;
    virtual void update(float dt) = 0;
    virtual void move(float dx, float dy) = 0;
    virtual void draw(sf::RenderWindow& window, float offsetX = 0, float offsetY = 0) = 0;
    
    virtual std::string getName() const = 0;
    virtual sf::Vector2f getPosition() const = 0;
    virtual float getLife() const = 0;
    virtual CollisionShape getCollisionBox() const = 0;
    virtual float getSpeed() const = 0;
    virtual float getSize() const = 0;
    virtual void takeDamage(float damage, bool isContinuous = false) = 0;
    virtual bool isAlive() const = 0;

    virtual ActorEffectComponent* getEffectComponent() = 0;
    virtual const ActorEffectComponent* getEffectComponent() const = 0;
    virtual void setFrozen(bool frozen) = 0;
    virtual bool isFrozen() const = 0;

protected:
    std::string name;
    sf::Vector2f position;
    float size;
    float damageCooldown;

    bool _isFrozen;
    CollisionShape collisionBox;
};

#endif // I_ACTOR_H