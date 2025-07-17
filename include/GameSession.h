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
    GameSession(Font& font, const Vector2u& winSize);

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

    Vector2u screenSize;

    bool debugHitboxes = false;

    void handleInputs(RenderWindow& window);
    void openShopMenu(RenderWindow& window);
    void drawDebugHitbox(RenderWindow& window, float x, float y, float radius, float offsetX = 0, float offsetY = 0);
    void debugHitboxesDisplay(RenderWindow& window, const Character& character, float offsetX = 0, float offsetY = 0, EnemiesManager* enemiesManager = nullptr, ProjectilesManager* projectilesManager = nullptr, const shared_ptr<Character>& player = nullptr);

};

#endif