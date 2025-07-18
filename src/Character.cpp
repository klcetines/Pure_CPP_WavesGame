#include "Character.h"
#include "Effect.h"

Character::Character(const string& name, float x, float y)
{
    _size = 40.0f;
    _name = name;
    
    if (SpriteLoader::assignSprite(_sprite, "assets/sprites/Characters/tomato_tv.png", _size * 2.0f, _size * 2.0f)) {
        _sprite.setPosition(x, y);
        _useSprite = true;
    } 
    else {
        shape.setRadius(_size);
        shape.setPointCount(30);
        shape.setFillColor(Color::Green);
        shape.setOrigin(_size, _size);
        shape.setPosition(x, y);
        _useSprite = false;
    }

    _position.x = x;
    _position.y = y;
    _data.Life = new Life(100.0f);
    _data.Speed = 5.0f;
    _data.Damage = 25.0f;
    _data.BulletsNumber = 1;
    _data.AttackSpeed = 1.0f;
    _data.ProjectileSpeed = 200.0f;
    _damageCooldown = 0.0f;
    _shootCooldown = 0.0f;
}

void Character::update(float dt) {
    updateCooldown(dt);
   
}

void Character::move(float dx, float dy) {
    _position.x += dx;
    _position.y += dy;
}

void Character::draw(RenderWindow& window, float offsetX, float offsetY) {
    if (_damageCooldown > 0) {
        int blink = static_cast<int>(_damageCooldown * 10) % 2;
        if (blink != 0) return;
    }
    if (_useSprite) {
        _sprite.setPosition(_position.x + offsetX, _position.y + offsetY);
        _sprite.setRotation(_facingAngle);    
        window.draw(_sprite);
    } else {
        shape.setPosition(_position.x + offsetX, _position.y + offsetY);
        window.draw(shape);
    }
}

shared_ptr<vector<shared_ptr<PlayerProjectile>>> Character::atack(const pair<float, float>& target) {
    vector<shared_ptr<PlayerProjectile>> projectiles;
    if (_shootCooldown > 0) {
        return make_shared<vector<shared_ptr<PlayerProjectile>>>(projectiles);
    }
    _shootCooldown = _data.AttackSpeed;

    Vector2f origin(_position.x, _position.y);
    Vector2f to(target.first, target.second);

    Vector2f dir = to - origin;
    float length = sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length == 0) length = 1;
    dir /= length;

    _facingAngle = atan2(dir.y, dir.x) * 180.0f / 3.14159265f - 90.0f;

    float spacing = 15.0f; 

    for (int i = 0; i < _data.BulletsNumber; i++) {
        Vector2f offset = dir * (i * spacing);
        Vector2f spawnPos = origin + offset;
        projectiles.push_back(make_shared<PlayerProjectile>(spawnPos, to, _data.ProjectileSpeed, _data.Damage));
    }
    return make_shared<vector<shared_ptr<PlayerProjectile>>>(projectiles);
}

string Character::getName() const {
    return _name;
}

pair<float,float> Character::getPosition() const {
    return {_position.x, _position.y};
}

void Character::handleCollisions(const vector<shared_ptr<Enemy>>& Enemies, float offsetX, float offsetY) {
    for (const auto& Enemy : Enemies) {
        auto enemyPosition = Enemy->getPosition();
        float dx = _position.x - enemyPosition.first;
        float dy = _position.y - enemyPosition.second;
        float distance = sqrt(dx * dx + dy * dy);
        if (Enemy->collidesWith(_position.x, _position.y) && _damageCooldown <= 0) {
            _position.x += dx * 0.1f;
            _position.y += dy * 0.1f;
            _data.Life->takeDamage(Enemy->getDamage());
            if (_data.Life->getLife() <= 0) {
                handleDead();
            }
            _damageCooldown = DAMAGE_COOLDOWN;
        }
    }
}

void Character::handleDead() {
    _data.Life->setLife(0);
    shape.setFillColor(Color::Red);
}

float Character::getLife() const {
    return _data.Life->getLife();
}

void Character::updateCooldown(float dt) {
    if (_damageCooldown > 0)
        _damageCooldown -= dt;
    if (_damageCooldown < 0)
        _damageCooldown = 0;
    
    if (_shootCooldown > 0)
        _shootCooldown -= dt;
    if (_shootCooldown < 0)
        _shootCooldown = 0;
}

float Character::getSize() const {
    return _size;
}

CharacterData Character::getData() const {
    return _data;
}

void Character::upgradeStats(const Effect& effect) {
    if (effect.getType() == "health") {
        _data.Life->heal(effect.getValue());
    } 
    else if (effect.getType() == "damage") {
        _data.Damage += effect.getValue();
    } 
    else if (effect.getType() == "speed") {
        _data.Speed += effect.getValue();
    }
    else if (effect.getType() == "attack_speed") {
        _data.AttackSpeed += effect.getValue();
    } 
    else if (effect.getType() == "bullets") {
        _data.BulletsNumber += static_cast<int>(effect.getValue());
    }
}