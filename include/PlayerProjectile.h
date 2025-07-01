#ifndef PLAYER_PROJECTILE_H
#define PLAYER_PROJECTILE_H

#include "Projectile.h"
#include <SFML/Graphics.hpp>
#include <cmath>

using namespace sf;
using namespace std;

class PlayerProjectile : public Projectile {
public:
    PlayerProjectile(sf::Vector2f start, sf::Vector2f target, float speed = 500.f);
    void update(float dt) override;
    void draw(RenderWindow& window, float offsetX, float offsetY);
    bool isAlive() const override;
    Vector2f getPosition() const override;

private:
    CircleShape shape;
    Vector2f position;
    Vector2f velocity;
    bool _alive;
    float _lifetime = 0.f;
};

#endif