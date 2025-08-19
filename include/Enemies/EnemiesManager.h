#ifndef ENEMIES_MANAGER_H
#define ENEMIES_MANAGER_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Enemies/Enemy.h"
#include "GameManagers/GameStatistics.h"

using namespace sf;
using namespace std;

class EnemiesManager {
    public:
        EnemiesManager(shared_ptr<GameStatistics> stats);
        void update(float dt, const Vector2f& playerPos);
        void draw(RenderWindow& window, float offsetX, float offsetY);
        void spawnEnemyNear(const Vector2f& playerPos);
        const vector<shared_ptr<Enemy>>& getEnemies() const;
        shared_ptr<Enemy> getClosestEnemy(const Vector2f& playerPos) const;

    private:
        shared_ptr<GameStatistics> stats;
        vector<shared_ptr<Enemy>> enemies;
        float spawnTimer;
        float spawnInterval;

        pair<float, float> getRandomSpawnPosition(const Vector2f& playerPos, float radius);
};

#endif 