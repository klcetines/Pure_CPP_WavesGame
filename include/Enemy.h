#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include "Position.h"
#include "Life.h"
#include "SpriteLoader.h"

using namespace sf;
using namespace std;

struct EnemyData
{
    Life* Life;
    float Speed;
    float Damage;
    float AttackSpeed;
};

class Enemy {
    public:
        Enemy(const string& name, float x, float y, float life = 100.0f);
        ~Enemy() = default;
        void move(float dx, float dy);
        void draw(RenderWindow& window, float offsetX, float offsetY);
        string getName() const;
        float getSpeed() const;
        float getDamage() const;
        pair<float, float> getPosition() const;
        float getWidth() const;
        float getHeight() const;
        EnemyData getData() const;
        bool collidesWith(float px, float py) const;
        float getRotation() const;

    private:
        string _name;
        CircleShape shape;
        Position _position;
        EnemyData _data;
        Sprite _sprite;

        bool _useSprite = false;
        Vector2f _lastMoveDir = {0.f, -1.f};
        float _facingAngle = 0.0f;
        Vector2u _size;
};      

#endif // ENEMY_H