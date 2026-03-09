#include "Projectiles/ProjectilesManager.h"
#include "Projectiles/Projectile.h"
#include "Projectiles/PlayerProjectile.h"
#include "Enemies/Enemy.h"
#include <vector>
#include <memory>

using namespace std;

ProjectilesManager::ProjectilesManager() {
}

void ProjectilesManager::add(shared_ptr<Projectile> proj)  {
    proj->setSpawnCallback([this](shared_ptr<Projectile> child) {
        this->_projectilesToAdd.push_back(child);
    });
    _projectilesToAdd.push_back(proj);
}

void ProjectilesManager::update(float dt, shared_ptr<Enemy> closestEnemy) {
    if (!_projectilesToAdd.empty()) {
        _projectiles.insert(_projectiles.end(), _projectilesToAdd.begin(), _projectilesToAdd.end());
        _projectilesToAdd.clear();
    }

    for (auto& p : _projectiles) {
        if (auto playerProj = dynamic_pointer_cast<PlayerProjectile>(p)) {
            playerProj->update(dt, closestEnemy);
        } else {
            p->update(dt);
        }
    }
    
    _projectiles.erase(std::remove_if(_projectiles.begin(), _projectiles.end(),
        [](const shared_ptr<Projectile>& p) { return !p->isAlive(); }),
        _projectiles.end());
}

void ProjectilesManager::draw(RenderWindow& window, float offsetX, float offsetY) {
    for (auto& p : _projectiles) {
        p->draw(window, offsetX, offsetY);
    }
}

const vector<shared_ptr<Projectile>>& ProjectilesManager::getProjectiles() const {
    return _projectiles;
}
