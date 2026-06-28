#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include "Utils/IActor.h"
#include "Utils/Position.h"
#include "Utils/Life.h"
#include "Utils/SpriteLoader.h"
#include "Utils/CollisionShape.h"
#include "Effects/CharacterEffects/ActorEffectComponent.h"



struct EnemyData
{
    Life* life;
    float Speed;
    float Damage;
    float AttackSpeed;
};

class ActorUI;

class Enemy : public IActor, public std::enable_shared_from_this<Enemy>{
    public:
        Enemy(const std::string& name, float x, float y, float life = 100.0f);
        ~Enemy() = default;

        void update(float dt);
        void move(float dx, float dy);
        void draw(sf::RenderWindow& window, float offsetX, float offsetY);

        int getId() const;
        std::string getName() const;
        float getSpeed() const override;
        float getDamage() const;
        sf::Vector2f getPosition() const;
        sf::Vector2f getHeadPosition() const;
        float getWidth() const;
        float getHeight() const;
        EnemyData getData() const;
        float getLife() const override;
        float getSize() const override;
        bool isAlive() const override;
        
        ActorEffectComponent* getEffectComponent() override;
        const ActorEffectComponent* getEffectComponent() const override;

        bool collidesWith(float px, float py) const;
        
        float getRotation() const;
        CollisionShape getCollisionBox() const;
        void takeDamage(float damage, bool isContinuous = false) override;
        void setFrozen(bool frozen) override;
        bool isFrozen() const override;
    
    private:
        int _id;
        static int _nextId;

        std::string _name;
        sf::CircleShape shape;
        Position _position;
        EnemyData _data;
        sf::Sprite _sprite;

        ActorEffectComponent _effectComponent;
        std::shared_ptr<ActorUI> _ui;

        bool _useSprite = false;
        sf::Vector2f _lastMoveDir = {0.f, -1.f};
        float _facingAngle = 0.0f;
        sf::Vector2u _size;
        CollisionShape _collisionBox;
        float _damageFlashTimer = 0.0f;

        bool _isFrozen;
        
        void applyKnockback();
};      

#endif // ENEMY_H