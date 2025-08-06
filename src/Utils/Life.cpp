#include "Utils/Life.h"

Life::Life(float maxLife)
    : life(maxLife), maxLife(maxLife) {}

void Life::takeDamage(float amount) {
    life -= amount;
    if (life < 0) life = 0;
}

void Life::heal(float amount) {
    life += amount;
    if (life > maxLife) life = maxLife;
}

float Life::getLife() const {
    return life;
}

float Life::getMaxLife() const {
    return maxLife;
}

bool Life::isAlive() const {
    return life > 0;
}

void Life::setLife(float value) {
    life = value;
    if (life > maxLife) life = maxLife;
    if (life < 0) life = 0;
}