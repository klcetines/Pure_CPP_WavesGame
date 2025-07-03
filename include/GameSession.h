#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "Character.h"
#include "EnemiesManager.h"
#include "ProjectilesManager.h"
#include "Background.h"

using namespace sf;
using namespace std;

struct GameSessionStatistics
{
    int enemiesKilled;
    int projectilesFired;
    int currency;
};


class GameSession {
public:
    GameSession(Font& font);

    void update(float dt);
    void render(RenderWindow& window);
    bool isPlayerDead() const;
    shared_ptr<Character> getPlayer() const;

private:
    shared_ptr<Character> player;
    shared_ptr<EnemiesManager> enemiesManager;
    shared_ptr<ProjectilesManager> projectilesManager;
    Background background;
    Text positionText;
    Text lifeText;
    Font& font;

    GameSessionStatistics stats;
};

#endif