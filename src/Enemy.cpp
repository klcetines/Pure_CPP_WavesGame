#include "Enemy.h"

Enemy::Enemy(const string& name, float x, float y, float life): _name(name), _position{x, y}
{
    shape.setRadius(18);
    shape.setPointCount(30);
    shape.setFillColor(Color::Red);
    shape.setPosition(x, y);

    _data.Speed = 2.0f;
    _data.Damage = 10.0f;
    _data.AttackSpeed = 1.0f;
    _data.Life = new Life(life);
}

void Enemy::move(float dx, float dy) {
    _position.x += dx;
    _position.y += dy;
    shape.setPosition(_position.x, _position.y);
}

void Enemy::draw(RenderWindow& window, float offsetX, float offsetY) {
    shape.setPosition(_position.x + offsetX, _position.y + offsetY);
    window.draw(shape);
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

float Enemy::getSize() const {
    return shape.getRadius();
}

float Enemy::getDamage() const {
    return _data.Damage;
}