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
#include "Rearrangement/Rearrange.h"
#include "Map/Map.h"
#include "Utils/Camera.h"


class GameSession {
public:
    GameSession(sf::Font& font, const sf::Vector2u& winSize);

    void update(float dt, sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
    bool isPlayerDead() const;
    std::shared_ptr<Character> getPlayer() const;
    void applyUpgrade(const Effect& effect);

private:
    std::shared_ptr<GameStatistics> stats;
    std::shared_ptr<Character> player;
    std::shared_ptr<EnemiesManager> enemiesManager;
    std::shared_ptr<ProjectilesManager> projectilesManager;
    std::shared_ptr<Shop> shop;
    std::shared_ptr<Map> map;
    std::unique_ptr<Camera> camera;
    
    Background background;
    sf::Text positionText;
    sf::Text lifeText;
    sf::Font& font;

    sf::Vector2u screenSize;

    bool debugHitboxes = false;

    void processPlayerInput(sf::RenderWindow& window);
    void processDebugInput(sf::RenderWindow& window);
    void processShopInput(sf::RenderWindow& window);
    void processRearrangementInput(sf::RenderWindow& window);

    void updatePlayer(float dt);

    void atackNearestEnemy();

    void updateEnemies(float dt);
    void updateProjectiles(float dt);
    void handlePlayerEnemyCollisions();
    void handleProjectileEnemyCollisions();

    void updateUITexts();
    void updatePlayerPositionText();
    void updatePlayerLifeText();

    void renderBackground(sf::RenderWindow& window);
    void renderEntities(sf::RenderWindow& window, sf::Vector2f offset = sf::Vector2f(0, 0));
    void renderUI(sf::RenderWindow& window);
    void renderDebug(sf::RenderWindow& window);

    void openShopMenu(sf::RenderWindow& window);

    void enemyCollidedByProjectile(std::shared_ptr<Enemy> enemy, std::shared_ptr<Projectile> projectile);

    void drawDebugHitbox(sf::RenderWindow& window, float x, float y, float radius, float offsetX = 0, float offsetY = 0);
    void drawDebugCapsule(sf::RenderWindow& window, const CollisionShape& capsule, float offsetX, float offsetY);
    void debugHitboxesDisplay(sf::RenderWindow& window, const Character& character, float offsetX = 0, float offsetY = 0, EnemiesManager* enemiesManager = nullptr, ProjectilesManager* projectilesManager = nullptr, const std::shared_ptr<Character>& player = nullptr);

    void RenderEffectsArrange(sf::RenderWindow& window);
    void drawEffectSlot(sf::RenderWindow& window, float x, float y, float size, const std::string& label, const sf::Color& color);

};

#endif