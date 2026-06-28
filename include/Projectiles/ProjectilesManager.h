#ifndef PROJECTILESMANAGER_H
#define PROJECTILESMANAGER_H

#include <vector>
#include <memory>
#include "Projectiles/Projectile.h"
#include "Enemies/Enemy.h"
#include <SFML/Graphics.hpp>


class ProjectilesManager {
public:
    ProjectilesManager();
    
    void add(std::shared_ptr<Projectile> proj);

    void update(float dt, std::shared_ptr<Enemy> closestEnemy = nullptr);

    void draw(sf::RenderWindow& window, float offsetX, float offsetY);

    const std::vector<std::shared_ptr<Projectile>>& getProjectiles() const;

private:
    std::vector<std::shared_ptr<Projectile>> _projectiles;
    std::vector<std::shared_ptr<Projectile>> _projectilesToAdd;
};

#endif