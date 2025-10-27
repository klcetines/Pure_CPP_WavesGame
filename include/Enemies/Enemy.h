#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include "Utils/IAnimatedObject.h"
#include "Utils/Position.h"
#include "Utils/Life.h"
#include "Utils/SpriteLoader.h"
#include "Utils/CollisionShape.h"


using namespace sf;
using namespace std;

struct EnemyData
{
    Life* Life;
    float Speed;
    float Damage;
    float AttackSpeed;
};

class Enemy : public IAnimatedObject {
    public:
        Enemy(const string& name, float x, float y, float life = 100.0f);
        ~Enemy() = default;

        void update(float dt);
        void move(float dx, float dy);
        void draw(RenderWindow& window, float offsetX, float offsetY);

        int getId() const;
        string getName() const;
        float getSpeed() const;
        float getDamage() const;
        Vector2f getPosition() const;
        Vector2f getHeadPosition() const;
        float getWidth() const;
        float getHeight() const;
        EnemyData getData() const;
        float getLife() const override;
        float getSize() const override;
        
        bool collidesWith(float px, float py) const;
        
        float getRotation() const;
        CollisionShape getCollisionBox() const;
        void takeDamage(float damage);
    
    private:
        int _id;
        static int _nextId;

        string _name;
        CircleShape shape;
        Position _position;
        EnemyData _data;
        Sprite _sprite;

        bool _useSprite = false;
        Vector2f _lastMoveDir = {0.f, -1.f};
        float _facingAngle = 0.0f;
        Vector2u _size;
        CollisionShape _collisionBox;
        float _damageFlashTimer = 0.0f;

        void applyKnockback();
};      

#endif // ENEMY_H