#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Position.h"
#include "Life.h"

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
        float getSize() const;
        EnemyData getData() const;

    private:
        string _name;
        CircleShape shape;
        Position _position;
        EnemyData _data;
};

#endif // ENEMY_H