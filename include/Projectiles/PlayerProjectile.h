#ifndef PLAYER_PROJECTILE_H
#define PLAYER_PROJECTILE_H

#include "Projectiles/Projectile.h"
#include "Utils/CollisionShape.h"
#include <SFML/Graphics.hpp>
#include <cmath>

using namespace sf;
using namespace std;

class PlayerProjectile : public Projectile {
public:
    PlayerProjectile(Vector2f start, Vector2f target, float speed = 200.0f, float damage = 10.0f);
    void update(float dt) override;
    void draw(RenderWindow& window, float offsetX, float offsetY);
    bool isAlive() const override;
    Vector2f getPosition() const override;
    float getDamage() const;
    float getSize() const override;
    void destroy();
    CollisionShape getCollisionBox() const;
    
private:
    CircleShape shape;
    Vector2f position;
    Vector2f velocity;
    bool _alive;
    float _lifetime = 0.f;
    float _damage;
    CollisionShape _collisionBox;
};

#endif