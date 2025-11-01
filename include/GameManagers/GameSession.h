#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "Character/Character.h"
#include "Enemies/EnemiesManager.h"
#include "Projectiles/ProjectilesManager.h"
#include "Utils/Background.h"
#include "GameManagers/GameStatistics.h"
#include "Shop/Shop.h"
#include "Utils/Effect.h"

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

    void processPlayerInput(RenderWindow& window);
    void processDebugInput(RenderWindow& window);
    void processShopInput(RenderWindow& window);

    void updatePlayer(float dt);

    void atackNearestEnemy();

    void updateEnemies(float dt);
    void updateProjectiles(float dt);
    void handlePlayerEnemyCollisions();
    void handleProjectileEnemyCollisions();

    void updateUITexts();
    void updatePlayerPositionText();
    void updatePlayerLifeText();

    void renderBackground(RenderWindow& window);
    void renderEntities(RenderWindow& window);
    void renderUI(RenderWindow& window);
    void renderDebug(RenderWindow& window);

    void openShopMenu(RenderWindow& window);

    void enemyCollidedByProjectile(shared_ptr<Enemy> enemy, shared_ptr<Projectile> projectile);

    void drawDebugHitbox(RenderWindow& window, float x, float y, float radius, float offsetX = 0, float offsetY = 0);
    void drawDebugCapsule(RenderWindow& window, Vector2f pointA, Vector2f pointB, float radius, float offsetX, float offsetY);
    void debugHitboxesDisplay(RenderWindow& window, const Character& character, float offsetX = 0, float offsetY = 0, EnemiesManager* enemiesManager = nullptr, ProjectilesManager* projectilesManager = nullptr, const shared_ptr<Character>& player = nullptr);

};

#endif