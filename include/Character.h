#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include "Position.h"
#include "Life.h"
#include "Enemy.h"
#include "PlayerProjectile.h"

using namespace sf;
using namespace std;

struct CharacterData
{
    Life* Life;
    float Speed;
    float Damage;
    int BulletsNumber;
    float AttackSpeed;
};

const float DAMAGE_COOLDOWN = 0.5f;

class Character {
    public:
        Character(const string& name, float x, float y);
        ~Character() = default;
        void update(float dt);
        void move(float dx, float dy);
        void draw(RenderWindow& window);
        shared_ptr<PlayerProjectile> atack(const pair<float, float>& target);
        string getName() const;
        pair<float, float> getPosition() const; // Añadido para obtener la posición
        void handleCollisions(const vector<shared_ptr<Enemy>>& Enemies, float offsetX, float offsetY);
        void handleDead();
        float getLife() const;
        float getSize() const;

    private:
        string _name;
        CircleShape shape;
        float _size;
        Position _position;
        CharacterData _data;
        float _damageCooldown;
        float _shootCooldown;

        void updateCooldown(float dt);

};

#endif