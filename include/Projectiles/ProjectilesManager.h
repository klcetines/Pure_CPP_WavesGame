#ifndef PROJECTILESMANAGER_H
#define PROJECTILESMANAGER_H

#include <vector>
#include <memory>
#include "Projectiles/Projectile.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class ProjectilesManager {
public:
    ProjectilesManager();
    
    void add(shared_ptr<Projectile> proj);

    void update(float dt);

    void draw(RenderWindow& window, float offsetX, float offsetY);

    const vector<shared_ptr<Projectile>>& getProjectiles() const;

private:
    vector<shared_ptr<Projectile>> _projectiles;
};

#endif