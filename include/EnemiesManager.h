#ifndef ENEMIES_MANAGER_H
#define ENEMIES_MANAGER_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Enemy.h"

using namespace sf;
using namespace std;

class EnemiesManager {
    public:
        EnemiesManager();
        void update(float dt, const pair<float, float>& playerPos);
        void draw(RenderWindow& window, float offsetX, float offsetY);
        void spawnEnemyNear(const pair<float, float>& playerPos);
        const vector<shared_ptr<Enemy>>& getEnemies() const;
        shared_ptr<Enemy> getClosestEnemy(const pair<float, float>& playerPos) const;

    private:
        vector<shared_ptr<Enemy>> enemies;
        float spawnTimer;
        float spawnInterval;

        pair<float, float> getRandomSpawnPosition(const pair<float, float>& playerPos, float radius);
};

#endif 