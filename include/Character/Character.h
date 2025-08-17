#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include "Utils/Position.h"
#include "Utils/Life.h"
#include "Enemies/Enemy.h"
#include "Projectiles/PlayerProjectile.h"
#include "Utils/SpriteLoader.h"
#include "Utils/CollisionShape.h"

using namespace sf;
using namespace std;

class Effect;

struct CharacterData
{
    Life* Life;
    float Speed;
    float Damage;
    int BulletsNumber;
    float AttackSpeed;
    float ProjectileSpeed;
};

const float DAMAGE_COOLDOWN = 0.5f;

class Character {
    public:
        Character(const string& name, float x, float y);
        ~Character() = default;
        void update(float dt);
        void move(float dx, float dy);
        void draw(RenderWindow& window, float offsetX = 0, float offsetY = 0);
        shared_ptr<vector<shared_ptr<PlayerProjectile>>> atack(const pair<float, float>& target);
        string getName() const;
        pair<float, float> getPosition() const;
        void handleCollisions(const vector<shared_ptr<Enemy>>& Enemies, float offsetX, float offsetY);
        void handleDead();
        float getLife() const;
        float getSize() const;
        CharacterData getData() const;
        void upgradeStats(const Effect& effect);
        CollisionShape getCollisionBox() const;

    private:
        string _name;
        CircleShape shape;
        float _size;
        Position _position;
        CharacterData _data;
        float _damageCooldown;
        float _shootCooldown;
        float _facingAngle;
        CollisionShape _collisionBox;

        Texture _texture;
        Sprite _sprite;
        bool _useSprite = false;
        
        void updateCooldown(float dt);
};

#endif