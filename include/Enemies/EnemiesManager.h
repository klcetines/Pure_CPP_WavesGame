#ifndef ENEMIES_MANAGER_H
#define ENEMIES_MANAGER_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Enemies/Enemy.h"
#include "GameManagers/GameStatistics.h"


class EnemiesManager {
    public:
        EnemiesManager(std::shared_ptr<GameStatistics> stats);
        void update(float dt, const sf::Vector2f& playerPos);
        void draw(sf::RenderWindow& window, float offsetX, float offsetY);
        void spawnEnemyNear(const sf::Vector2f& playerPos);
        const std::vector<std::shared_ptr<Enemy>>& getEnemies() const;
        std::shared_ptr<Enemy> getClosestEnemy(const sf::Vector2f& playerPos) const;
        void toggleTestMode(const sf::Vector2f& playerPos);

    private:
        std::shared_ptr<GameStatistics> stats;
        std::vector<std::shared_ptr<Enemy>> enemies;
        float spawnTimer;
        float spawnInterval;
        bool _testMode;

        std::pair<float, float> getRandomSpawnPosition(const sf::Vector2f& playerPos, float radius);
};

#endif 