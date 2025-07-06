#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "Character.h"
#include "EnemiesManager.h"
#include "ProjectilesManager.h"
#include "Background.h"
#include "GameStatistics.h"
#include "Shop.h"
#include "Effect.h"

using namespace sf;
using namespace std;

class GameSession {
public:
    GameSession(Font& font);

    void update(float dt, RenderWindow& window);
    void render(RenderWindow& window);
    bool isPlayerDead() const;
    shared_ptr<Character> getPlayer() const;
    void applyUpgrade(const Effect& effect);

private:
    shared_ptr<GameStatistics> stats;
    shared_ptr<Character> player;
    shared_ptr<EnemiesManager> enemiesManager;
    shared_ptr<ProjectilesManager> projectilesManager;
    shared_ptr<Shop> shop;
    Background background;
    Text positionText;
    Text lifeText;
    Font& font;

    void handleInputs(RenderWindow& window);
    void openShopMenu(RenderWindow& window);

};

#endif