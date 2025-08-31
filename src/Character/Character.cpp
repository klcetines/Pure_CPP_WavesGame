#include "Character/Character.h"

Character::Character(const string& name, float x, float y)
    : name(name)
    , position(x, y)
    , size(DEFAULT_SIZE)
    , damageCooldown(0.0f)
    , stats()
    , graphics(size)
    , combat(stats)
    , collisionBox(Vector2f(x, y), size * 1.6f)
{
    graphics.loadSprite("assets/sprites/Characters/tomato_tv.png", x, y);
}

void Character::update(float dt) {
    if (damageCooldown > 0) {
        damageCooldown = max(0.0f, damageCooldown - dt);
    }
    combat.updateCooldowns(dt);
}

void Character::move(float dx, float dy) {
    position.x += dx;
    position.y += dy;
    collisionBox.center = position;
}

void Character::draw(RenderWindow& window, float offsetX, float offsetY) {
    graphics.draw(window, position, combat.getFacingAngle(), 
                 (damageCooldown > 0), offsetX, offsetY);
}

shared_ptr<vector<shared_ptr<PlayerProjectile>>> Character::attack(const Vector2f& target) {
    return combat.createAttack(position, target);
}

void Character::handleCollisions(const vector<shared_ptr<Enemy>>& enemies, 
                               float offsetX, float offsetY) 
{
    for (const auto& enemy : enemies) {
        if (collisionBox.intersects(enemy->getCollisionBox()) && damageCooldown <= 0) {
            handleDamage(enemy);
        }
    }
}

void Character::handleDamage(shared_ptr<Enemy> enemy) {
    stats.getLife().takeDamage(enemy->getDamage());
    damageCooldown = DAMAGE_COOLDOWN;
    applyKnockback(enemy);
    
    if (stats.getLifeAmmount() <= 0) {
        graphics.setDeadAppearance();
    }
}

void Character::applyKnockback(shared_ptr<Enemy> enemy) {
    Vector2f enemyPos = enemy->getPosition();
    Vector2f direction = enemyPos - position;
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (length > 0) {
        direction /= length;
        move(-direction.x * KNOCKBACK_DISTANCE, -direction.y * KNOCKBACK_DISTANCE);
    }
}

string Character::getName() const { return name; }
Vector2f Character::getPosition() const { return position; }
float Character::getLife() const { return stats.getLifeAmmount(); }
CharacterStats& Character::getStats() { return stats; }
CollisionShape Character::getCollisionBox() const { return collisionBox; }
float Character::getSize() const { return size; }