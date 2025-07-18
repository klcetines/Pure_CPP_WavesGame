#include "Enemy.h"

Enemy::Enemy(const string& name, float x, float y, float life): _name(name), _position{x, y}
{
    _size.x = 56.0f;
    _size.y = 108.0f;

    if (SpriteLoader::assignSprite(_sprite, "assets/sprites/Enemies/rat_tv.png", _size.x, _size.y)) {
        Vector2u texSize = _sprite.getTexture()->getSize();
        _sprite.setOrigin(texSize.x / 2.0f, texSize.y / 2.0f);
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

    _data.Speed = 2.0f;
    _data.Damage = 10.0f;
    _data.AttackSpeed = 1.0f;
    _data.Life = new Life(life);
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
}

void Enemy::draw(RenderWindow& window, float offsetX, float offsetY) {
    if (_useSprite) {
        // Ángulo según la dirección de movimiento
        _facingAngle = atan2(_lastMoveDir.y, _lastMoveDir.x) * 180.0f / 3.14159265f - 90.0f;

        _sprite.setPosition(_position.x + offsetX, _position.y + offsetY);
        _sprite.setRotation(_facingAngle);
        window.draw(_sprite);
    } 
    else {
        shape.setPosition(_position.x + offsetX, _position.y + offsetY);
        window.draw(shape);
    }
}

string Enemy::getName() const {
    return _name;
}

float Enemy::getSpeed() const {
    return _data.Speed;
}

pair<float, float> Enemy::getPosition() const {
    return {_position.x, _position.y};
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

bool Enemy::collidesWith(float px, float py) const {
    float cx = _position.x;
    float cy = _position.y;
    float angleRad = _facingAngle * 3.14159265f / 180.0f;

    // Traslada el punto al origen del enemigo
    float relX = px - cx;
    float relY = py - cy;

    // Rota el punto en sentido inverso al enemigo
    float rotX =  relX * cos(-angleRad) - relY * sin(-angleRad);
    float rotY =  relX * sin(-angleRad) + relY * cos(-angleRad);

    float halfH = (_size.y * 0.45f) / 2.0f;
    float halfW = (_size.x * 0.65f) / 2.0f;

    // Rectángulo central
    if (rotX > -halfW && rotX < halfW &&
        rotY > -halfH && rotY < halfH) {
        return true;
    }
    // Círculo superior
    float dxT = rotX;
    float dyT = rotY + halfH;
    if (dxT*dxT + dyT*dyT < halfW*halfW) return true;
    // Círculo inferior
    float dxB = rotX;
    float dyB = rotY - halfH;
    if (dxB*dxB + dyB*dyB < halfW*halfW) return true;

    return false;
}

float Enemy::getRotation() const {
    return _facingAngle;
}