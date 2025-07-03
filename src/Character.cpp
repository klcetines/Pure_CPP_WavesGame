#include "Character.h"

Character::Character(const string& name, float x, float y)
{
    _size = 20.0f;
    _name = name;
    shape.setRadius(20);
    shape.setPointCount(30);
    shape.setFillColor(Color::Green);
    shape.setPosition(x, y);

    shape.setOrigin(_size, _size);

    _position.x = 0;
    _position.y = 0;
    _data.Life = new Life(100.0f);
    _data.Speed = 5.0f;
    _data.Damage = 50.0f;
    _data.BulletsNumber = 1;
    _data.AttackSpeed = 1.0f;
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

void Character::draw(RenderWindow& window) {
    if (_damageCooldown > 0) {
        int blink = static_cast<int>(_damageCooldown * 10) % 2;
        if (blink == 0) {
            window.draw(shape);
        }
    } else {
        window.draw(shape);
    }
}

shared_ptr<PlayerProjectile> Character::atack(const pair<float, float>& target) {
    if (_shootCooldown <= 0) {
        _shootCooldown = _data.AttackSpeed;
        Vector2f from(_position.x, _position.y);
        Vector2f to(target.first, target.second);
        return make_shared<PlayerProjectile>(from, to, 200, _data.Damage);
    }
    return nullptr;
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
        if (distance < _size + Enemy->getSize() && _damageCooldown <= 0) {
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