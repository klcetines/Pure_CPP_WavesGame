#include "Enemies/EnemiesManager.h"
#include <cmath>
#include <cstdlib>

EnemiesManager::EnemiesManager(shared_ptr<GameStatistics> stats)
    : stats(stats), spawnTimer(0), spawnInterval(2.0f){}

void EnemiesManager::update(float dt, const Vector2f& playerPos) {

    spawnTimer += dt;
    if (spawnTimer >= spawnInterval) {
        spawnEnemyNear(playerPos);
        spawnTimer = 0;
    }

    for (auto& enemy : enemies) {
        enemy->_damageFlashTimer = max(0.0f, enemy->_damageFlashTimer - dt);
        auto epos = enemy->getPosition();
        float dx = playerPos.x - epos.x;
        float dy = playerPos.y - epos.y;
        float dist = sqrt(dx*dx + dy*dy);
        if (dist > 30.0f) {
            float vx = (dx / dist) * enemy->getSpeed();
            float vy = (dy / dist) * enemy->getSpeed();
            enemy->move(vx, vy);
        }
    }

    auto it = remove_if(enemies.begin(), enemies.end(),
        [this](const shared_ptr<Enemy>& e) {
            if (e->getData().Life->getLife() <= 0) {
                if (stats) stats->addKill();
                return true;
            }
            return false;
        });
    enemies.erase(it, enemies.end());

}

void EnemiesManager::draw(RenderWindow& window, float offsetX, float offsetY) {
    for (auto& enemy : enemies) {
        enemy->draw(window, offsetX, offsetY);
    }
}

void EnemiesManager::spawnEnemyNear(const Vector2f& playerPos) {
    auto pos = getRandomSpawnPosition(playerPos, 250.0f);
    auto enemy = make_shared<Enemy>("Enemy", pos.first, pos.second);
    enemies.push_back(enemy);
}

pair<float, float> EnemiesManager::getRandomSpawnPosition(const Vector2f& playerPos, float radius) {
    float angle = static_cast<float>(rand()) / RAND_MAX * 2 * 3.14159265f;
    float r = radius + static_cast<float>(rand()) / RAND_MAX * 50.0f;

    float x = playerPos.x + cos(angle) * r;
    float y = playerPos.y + sin(angle) * r;

    return {x, y};
}

const vector<shared_ptr<Enemy>>& EnemiesManager::getEnemies() const {
    return enemies;
}

shared_ptr<Enemy> EnemiesManager::getClosestEnemy(const Vector2f& playerPos) const {
    shared_ptr<Enemy> closest = nullptr;
    float minDist = numeric_limits<float>::max();
    for (const auto& enemy : enemies) {
        auto epos = enemy->getPosition();
        float dx = playerPos.x - epos.x;
        float dy = playerPos.y - epos.y;
        float dist = sqrt(dx*dx + dy*dy);
        if (dist < minDist) {
            minDist = dist;
            closest = enemy;
        }
    }
    return closest;
}
