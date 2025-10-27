#include "Enemies/Enemy.h"

int Enemy::_nextId = 1;

Enemy::Enemy(const string& name, float x, float y, float life):
    _name(name),
    _position{x, y}, 
    _id(_nextId++)
{
    _size.x = 56.0f;
    _size.y = 108.0f;

    if (SpriteLoader::assignSprite(_sprite, "assets/sprites/Enemies/rat_tv.png", _size.x, _size.y)) {
        Vector2u texSize = _sprite.getTexture()->getSize();
        _sprite.setOrigin(texSize.x / 2.0f, texSize.y / 4.0f);
        _sprite.setPosition(x, y);
        _useSprite = true;
    } 
    else {
        shape.setRadius(_size.x);
        shape.setPointCount(30);
        shape.setFillColor(Color::Red);
        shape.setOrigin(_size.x/2, _size.y/2);
        shape.setPosition(x, y);
        _useSprite = false;
    }

    _collisionBox = CollisionShape(Vector2f(x, y), _size.x/2.5, _size.y/2.5, 0.0f);
    _data.Speed = 2.0f;
    _data.Damage = 10.0f;
    _data.AttackSpeed = 1.0f;
    _data.Life = new Life(life);
}

void Enemy::update(float dt) {
    _damageFlashTimer = max(0.0f, _damageFlashTimer - dt);
}

void Enemy::move(float dx, float dy) {
    _position.x += dx;
    _position.y += dy;
    shape.setPosition(_position.x, _position.y);

    if (dx != 0.f || dy != 0.f) {
        float len = sqrt(dx*dx + dy*dy);
        if (len > 0.001f) {
            _lastMoveDir = {dx / len, dy / len};
        }
    }
    _collisionBox.center = Vector2f(_position.x, _position.y);
    _collisionBox.rotationDeg = atan2(_lastMoveDir.y, _lastMoveDir.x) * 180.0f / 3.14159265f;
}

void Enemy::draw(RenderWindow& window, float offsetX, float offsetY) {
    if (_useSprite) {
        _facingAngle = atan2(_lastMoveDir.y, _lastMoveDir.x) * 180.0f / 3.14159265f - 90.0f;
        if(_damageFlashTimer > 0.0f){
            _sprite.setColor(Color(255, 0, 0, 128));
        }
        else{
            _sprite.setColor(Color::White);
        }
        _sprite.setPosition(_position.x + offsetX, _position.y + offsetY);
        _sprite.setRotation(_facingAngle);
        window.draw(_sprite);
    } 
    else {
        shape.setPosition(_position.x + offsetX, _position.y + offsetY);
        window.draw(shape);
    }
}

int Enemy::getId() const {
    return _id;
}

string Enemy::getName() const {
    return _name;
}

float Enemy::getSpeed() const {
    return _data.Speed;
}

Vector2f Enemy::getPosition() const {
    return {_position.x, _position.y};
}

Vector2f Enemy::getHeadPosition() const {
    Vector2f dir = _lastMoveDir;
    float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
    if (len < 1e-6f) {
        dir = {0.0f, -1.0f};
    } else {
        dir.x /= len;
        dir.y /= len;
    }
    float offset = 3*(_size.y/4.0f);
    return {_position.x + dir.x * offset, _position.y + dir.y * offset};
}

float Enemy::getWidth() const {
    return _size.x;
}

float Enemy::getHeight() const {
    return _size.y;
}

float Enemy::getDamage() const {
    return _data.Damage;
}

EnemyData Enemy::getData() const {
    return _data;
}

float Enemy::getRotation() const {
    return _facingAngle;
}

CollisionShape Enemy::getCollisionBox() const {
    return _collisionBox;
}

float Enemy::getLife() const {
    return _data.Life->getLife();
}

float Enemy::getSize() const {
    return _size.x;
}

void Enemy::takeDamage(float damage) {
    _data.Life->takeDamage(damage);
    _damageFlashTimer = 0.1f;
    applyKnockback();
}

void Enemy::applyKnockback() {
    float knockbackDistance = 25.0f;
    _position.x -= _lastMoveDir.x * knockbackDistance;
    _position.y -= _lastMoveDir.y * knockbackDistance;
    shape.setPosition(_position.x, _position.y);
    _collisionBox.center = Vector2f(_position.x, _position.y);
}