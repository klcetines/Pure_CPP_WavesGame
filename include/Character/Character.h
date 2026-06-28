#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

#include "Utils/IActor.h"
#include "Character/CharacterStats.h"
#include "Character/CharacterGraphics.h"
#include "Character/CharacterCombat.h"
#include "Utils/CollisionShape.h"
#include "Projectiles/PlayerProjectile.h"
#include "Enemies/Enemy.h"


class Character : public IActor {
public:
    Character(const std::string& name, float x, float y);
    
    void update(float dt);
    void move(float dx, float dy);
    void draw(sf::RenderWindow& window, float offsetX = 0, float offsetY = 0);
    std::shared_ptr<std::vector<std::shared_ptr<PlayerProjectile>>> attack(const sf::Vector2f& target);
    
    void handleCollisions(const std::vector<std::shared_ptr<Enemy>>& enemies, 
                         float offsetX, float offsetY);

    ActorEffectComponent* getEffectComponent() override;
    const ActorEffectComponent* getEffectComponent() const override;
                         
    void takeDamage(float damage, bool isContinuous = false) override;
    std::string getName() const;
    sf::Vector2f getPosition() const;
    float getLife() const;
    CharacterStats& getStats();
    CollisionShape getCollisionBox() const;
    float getSize() const;
    float getSpeed() const override;
    bool isAlive() const override;
    void setFrozen(bool frozen) override;
    bool isFrozen() const override;

private:
    static constexpr float DEFAULT_SIZE = 40.0f;
    static constexpr float DAMAGE_COOLDOWN = 1.0f;
    static constexpr float KNOCKBACK_DISTANCE = 25.0f;

    std::string name;
    sf::Vector2f position;
    float size;
    float damageCooldown;

    ActorEffectComponent _effectComponent;

    CharacterStats stats;
    CharacterGraphics graphics;
    CharacterCombat combat;
    CollisionShape collisionBox;

    void handleDamage(std::shared_ptr<Enemy> enemy);
    void applyKnockback(std::shared_ptr<Enemy> enemy);
};

#endif // CHARACTER_H