#include "GameManagers/GameSession.h"

GameSession::GameSession(Font& font, const Vector2u& winSize)
    : font(font),
      background("assets/textures/cobblestone_1.png"),
      screenSize(winSize)
{
    stats = make_shared<GameStatistics>();
    player = make_shared<Character>("TouMate", winSize.x / 2, winSize.y / 2);
    enemiesManager = make_shared<EnemiesManager>(stats);
    projectilesManager = make_shared<ProjectilesManager>();
    shop = make_shared<Shop>(stats, *this);
    positionText = Text("Pos: (0, 0)", font, 20);
    positionText.setFillColor(Color::White);
    lifeText = Text("Life: 100", font, 20);
    lifeText.setFillColor(Color::Red);
}

void GameSession::update(float dt, RenderWindow& window) {
    handleInputs(window);

    auto posPlayer = player->getPosition();
    float offsetX = screenSize.x / 2 - posPlayer.first;
    float offsetY = screenSize.y / 2 - posPlayer.second;

    enemiesManager->update(dt, posPlayer);

    player->update(dt);
    player->handleCollisions(enemiesManager->getEnemies(), offsetX, offsetY);

    int life = static_cast<int>(player->getLife());
    lifeText.setString("Life: " + to_string(life));
    lifeText.setPosition(10, 10);

    positionText.setString("Pos: (" + to_string((int)posPlayer.first) + ", " + to_string((int)posPlayer.second) + ")");
    FloatRect textRect = positionText.getLocalBounds();
    positionText.setPosition(screenSize.x - textRect.width - 10, screenSize.y - textRect.height - 10);

    auto closest = enemiesManager->getClosestEnemy(player->getPosition());
    if (closest) {
        auto proj = player->atack(closest->getPosition());
        if (proj && !proj->empty()) {
            for (const auto& p : *proj) {
                projectilesManager->add(p);
            }
            stats->addProjectile();
        }
    }
    projectilesManager->update(dt);

    auto& projectiles = projectilesManager->getProjectiles();
    auto& enemies = enemiesManager->getEnemies();
    for (auto& enemy : enemies) {
        for (auto& proj : projectiles) {
            auto ppos = proj->getPosition();
            if (enemy->collidesWith(ppos.x, ppos.y)) {
                enemy->getData().Life->takeDamage(proj->getDamage());
                proj->destroy();
            }
        }
    }
}

void GameSession::render(RenderWindow& window) {
    auto posPlayer = player->getPosition();
    float offsetX = screenSize.x / 2 - posPlayer.first;
    float offsetY = screenSize.y / 2 - posPlayer.second;

    background.drawTiled(window, posPlayer.first, posPlayer.second, screenSize.x, screenSize.y);
    player->draw(window, offsetX, offsetY);
    enemiesManager->draw(window, offsetX, offsetY);
    projectilesManager->draw(window, offsetX, offsetY);
    window.draw(positionText);
    window.draw(lifeText);

    string statsStr = 
    "Currency: " + to_string(stats->getCurrency());

    Text statsText(statsStr, font, 18);
    statsText.setFillColor(Color::Cyan);
    statsText.setPosition(10, 40);
    window.draw(statsText);

    if (debugHitboxes) {
        debugHitboxesDisplay(
            window, *player, 
            screenSize.x / 2 - player->getPosition().first, 
            screenSize.y / 2 - player->getPosition().second, 
            enemiesManager.get(), 
            projectilesManager.get(), 
            player
        );
    }
}

bool GameSession::isPlayerDead() const {
    return player->getLife() <= 0;
}

void GameSession::handleInputs(RenderWindow& window) {
    float speed = player->getData().Speed;
    if (Keyboard::isKeyPressed(Keyboard::W)) player->move(0, -speed);
    if (Keyboard::isKeyPressed(Keyboard::S)) player->move(0, speed);
    if (Keyboard::isKeyPressed(Keyboard::A)) player->move(-speed, 0);
    if (Keyboard::isKeyPressed(Keyboard::D)) player->move(speed, 0);

    if (Keyboard::isKeyPressed(Keyboard::Space)) {
        openShopMenu(window);
    }

    static bool debugTogglePressed = false;
    if (Keyboard::isKeyPressed(Keyboard::LControl) &&
        Keyboard::isKeyPressed(Keyboard::LShift) &&
        Keyboard::isKeyPressed(Keyboard::D)) {
        if (!debugTogglePressed) {
            debugHitboxes = !debugHitboxes;
            debugTogglePressed = true;
        }
    } 
    else {
        debugTogglePressed = false;
    }
}

void GameSession::openShopMenu(RenderWindow& window) {
    if (shop) {
        shop->open(window, font);
    }
}

void GameSession::applyUpgrade(const Effect& effect) {
    if (player) {
        player->upgradeStats(effect);
    }
}

void GameSession::drawDebugHitbox(RenderWindow& window, float x, float y, float radius, float offsetX, float offsetY) {
    CircleShape hitbox(radius);
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Blue);
    hitbox.setOutlineThickness(2.f);
    hitbox.setOrigin(radius, radius);
    hitbox.setPosition(x + offsetX, y + offsetY);
    window.draw(hitbox);

    CircleShape point(3);
    point.setFillColor(sf::Color::Yellow);
    point.setOrigin(3, 3);
    point.setPosition(x + offsetX, y + offsetY);
    window.draw(point);
}

void GameSession::drawDebugCapsule(RenderWindow& window, float x, float y, float width, float height, float offsetX, float offsetY, float rotationDeg) {
    RectangleShape rect(sf::Vector2f(width, height));
    rect.setOrigin(width / 2, height / 2);
    rect.setPosition(x + offsetX, y + offsetY);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Blue);
    rect.setOutlineThickness(2.f);
    rect.setRotation(rotationDeg);
    window.draw(rect);

    float radius = width / 2;
    float angleRad = rotationDeg * 3.14159265f / 180.0f;

    // Calcula el desplazamiento de los extremos rotados
    float dx = sin(angleRad) * (height / 2);
    float dy = -cos(angleRad) * (height / 2);

    // Top circle (superior)
    CircleShape top(radius);
    top.setOrigin(radius, radius);
    top.setPosition(x + offsetX + dx, y + offsetY + dy);
    top.setFillColor(sf::Color::Transparent);
    top.setOutlineColor(sf::Color::Blue);
    top.setOutlineThickness(2.f);
    window.draw(top);

    // Bottom circle (inferior)
    CircleShape bottom(radius);
    bottom.setOrigin(radius, radius);
    bottom.setPosition(x + offsetX - dx, y + offsetY - dy);
    bottom.setFillColor(sf::Color::Transparent);
    bottom.setOutlineColor(sf::Color::Blue);
    bottom.setOutlineThickness(2.f);
    window.draw(bottom);
}

void GameSession::debugHitboxesDisplay(RenderWindow& window, const Character& character, float offsetX, float offsetY, EnemiesManager* enemiesManager, ProjectilesManager* projectilesManager, const shared_ptr<Character>& player) {
    drawDebugHitbox(window, screenSize.x / 2, screenSize.y / 2, player->getSize());

    for (const auto& enemy : enemiesManager->getEnemies()) {
        auto epos = enemy->getPosition();
        drawDebugCapsule(window, epos.first + offsetX, epos.second + offsetY, enemy->getWidth() * 0.65f, enemy->getHeight() * 0.45f, 0, 0, enemy->getRotation());    
    }

    for (const auto& proj : projectilesManager->getProjectiles()) {
        auto ppos = proj->getPosition();
        drawDebugHitbox(window, ppos.x + offsetX, ppos.y + offsetY, proj->getSize());
    }
}