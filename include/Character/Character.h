#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

#include "Character/CharacterStats.h"
#include "Character/CharacterGraphics.h"
#include "Character/CharacterCombat.h"
#include "Utils/CollisionShape.h"
#include "Projectiles/PlayerProjectile.h"
#include "Enemies/Enemy.h"

using namespace sf;
using namespace std;

class Character {
public:
    Character(const string& name, float x, float y);
    
    void update(float dt);
    void move(float dx, float dy);
    void draw(RenderWindow& window, float offsetX = 0, float offsetY = 0);
    shared_ptr<vector<shared_ptr<PlayerProjectile>>> attack(const Vector2f& target);
    
    void handleCollisions(const vector<shared_ptr<Enemy>>& enemies, 
                         float offsetX, float offsetY);
    
    string getName() const;
    Vector2f getPosition() const;
    float getLife() const;
    CharacterStats& getStats();
    CollisionShape getCollisionBox() const;
    float getSize() const;

private:
    static constexpr float DEFAULT_SIZE = 40.0f;
    static constexpr float DAMAGE_COOLDOWN = 1.0f;
    static constexpr float KNOCKBACK_DISTANCE = 25.0f;

    string name;
    Vector2f position;
    float size;
    float damageCooldown;

    CharacterStats stats;
    CharacterGraphics graphics;
    CharacterCombat combat;
    CollisionShape collisionBox;

    void handleDamage(shared_ptr<Enemy> enemy);
    void applyKnockback(shared_ptr<Enemy> enemy);
};