#include "EnemiesManager.h"
#include <cmath>
#include <cstdlib>

EnemiesManager::EnemiesManager()
    : spawnTimer(0), spawnInterval(2.0f){}

void EnemiesManager::update(float dt, const pair<float, float>& playerPos) {

    spawnTimer += dt;
    if (spawnTimer >= spawnInterval and enemies.size() < 25) {
        spawnEnemyNear(playerPos);
        spawnTimer = 0;
    }

    for (auto& enemy : enemies) {
        auto epos = enemy->getPosition();
        float dx = playerPos.first - epos.first;
        float dy = playerPos.second - epos.second;
        float dist = sqrt(dx*dx + dy*dy);
        if (dist > 30.0f) {
            float vx = (dx / dist) * enemy->getSpeed();
            float vy = (dy / dist) * enemy->getSpeed();
            enemy->move(vx, vy);
        }
    }
}

void EnemiesManager::draw(RenderWindow& window, float offsetX, float offsetY) {
    for (auto& enemy : enemies)
        enemy->draw(window, offsetX, offsetY);
}

void EnemiesManager::spawnEnemyNear(const pair<float, float>& playerPos) {
    auto pos = getRandomSpawnPosition(playerPos, 100.0f);
    auto enemy = make_shared<Enemy>("Enemy", pos.first, pos.second);
    enemies.push_back(enemy);
}

pair<float, float> EnemiesManager::getRandomSpawnPosition(const pair<float, float>& playerPos, float radius) {
    float angle = static_cast<float>(rand()) / RAND_MAX * 2 * 3.14159265f;
    float r = radius + static_cast<float>(rand()) / RAND_MAX * 50.0f;

    float x = playerPos.first + cos(angle) * r;
    float y = playerPos.second + sin(angle) * r;

    return {x, y};
}

const vector<shared_ptr<Enemy>>& EnemiesManager::getEnemies() const {
    return enemies;
}

shared_ptr<Enemy> EnemiesManager::getClosestEnemy(const pair<float, float>& playerPos) const {
    shared_ptr<Enemy> closest = nullptr;
    float minDist = numeric_limits<float>::max();
    for (const auto& enemy : enemies) {
        auto epos = enemy->getPosition();
        float dx = playerPos.first - epos.first;
        float dy = playerPos.second - epos.second;
        float dist = sqrt(dx*dx + dy*dy);
        if (dist < minDist) {
            minDist = dist;
            closest = enemy;
        }
    }
    return closest;
}
