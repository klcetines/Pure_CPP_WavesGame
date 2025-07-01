#include "ProjectilesManager.h"
#include "Projectile.h"
#include <vector>
#include <memory>

ProjectilesManager::ProjectilesManager() {
}

void ProjectilesManager::add(shared_ptr<Projectile> proj)  {
    projectiles.push_back(proj);
}

void ProjectilesManager::update(float dt) {
    for (auto& p : projectiles) p->update(dt);
        projectiles.erase( remove_if(projectiles.begin(), projectiles.end(),
                [](const shared_ptr<Projectile>& p) { 
                    return !p->isAlive(); 
                }),
            projectiles.end());
}

void ProjectilesManager::draw(RenderWindow& window, float offsetX, float offsetY) {
    for (auto& p : projectiles) p->draw(window, offsetX, offsetY);
}
