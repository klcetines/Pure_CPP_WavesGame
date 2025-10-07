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
    processPlayerInput(window);
    processDebugInput(window);
    processShopInput(window);

    updatePlayer(dt);
    updateEnemies(dt);
    updateProjectiles(dt);

    handlePlayerEnemyCollisions();
    handleProjectileEnemyCollisions();

    updateUITexts();
}

void GameSession::processPlayerInput(RenderWindow& window) {
    float speed = player->getStats().getSpeed();
    if (Keyboard::isKeyPressed(Keyboard::W)) player->move(0, -speed);
    if (Keyboard::isKeyPressed(Keyboard::S)) player->move(0, speed);
    if (Keyboard::isKeyPressed(Keyboard::A)) player->move(-speed, 0);
    if (Keyboard::isKeyPressed(Keyboard::D)) player->move(speed, 0);
}

void GameSession::processDebugInput(RenderWindow& window) {
    static bool debugTogglePressed = false;
    if (Keyboard::isKeyPressed(Keyboard::LControl) &&
        Keyboard::isKeyPressed(Keyboard::LShift) &&
        Keyboard::isKeyPressed(Keyboard::D)) {
        if (!debugTogglePressed) {
            debugHitboxes = !debugHitboxes;
            debugTogglePressed = true;
        }
    } else {
        debugTogglePressed = false;
    }
}

void GameSession::processShopInput(RenderWindow& window) {
    if (Keyboard::isKeyPressed(Keyboard::Space)) {
        openShopMenu(window);
    }
}

void GameSession::updatePlayer(float dt) {
    player->update(dt);
    atackNearestEnemy();
}

void GameSession::atackNearestEnemy() {
    auto closest = enemiesManager->getClosestEnemy(player->getPosition());
    if (closest) {
        auto proj = player->attack(closest->getPosition());
        if (proj && !proj->empty()) {
            for (const auto& p : *proj) {
                projectilesManager->add(p);
            }
            stats->addProjectile();
        }
    }
}

void GameSession::updateEnemies(float dt) {
    auto posPlayer = player->getPosition();
    enemiesManager->update(dt, posPlayer);
}

void GameSession::updateProjectiles(float dt) {
    projectilesManager->update(dt, enemiesManager->getClosestEnemy(player->getPosition()));
}

void GameSession::handlePlayerEnemyCollisions() {
    auto posPlayer = player->getPosition();
    float offsetX = screenSize.x / 2 - posPlayer.x;
    float offsetY = screenSize.y / 2 - posPlayer.y;
    player->handleCollisions(enemiesManager->getEnemies(), offsetX, offsetY);
}

void GameSession::handleProjectileEnemyCollisions() {
    auto& projectiles = projectilesManager->getProjectiles();
    auto& enemies = enemiesManager->getEnemies();
    for (auto& enemy : enemies) {
        const CollisionShape& enemyBox = enemy->getCollisionBox();
        for (auto& proj : projectiles) {

            const CollisionShape& projBox = proj->getCollisionBox();

            if (enemyBox.intersects(projBox)) {
                enemyCollidedByProjectile(enemy, proj);
            }
        }
    }
}

void GameSession::updateUITexts() {
    updatePlayerLifeText();
    updatePlayerPositionText();
}

void GameSession::updatePlayerLifeText() {
    int life = static_cast<int>(player->getLife());
    lifeText.setString("Life: " + to_string(life));
    lifeText.setPosition(10, 10);
}

void GameSession::updatePlayerPositionText() {
    auto posPlayer = player->getPosition();
    positionText.setString("Pos: (" + to_string((int)posPlayer.x) + ", " + to_string((int)posPlayer.y) + ")");
    FloatRect textRect = positionText.getLocalBounds();
    positionText.setPosition(screenSize.x - textRect.width - 10, screenSize.y - textRect.height - 10);
}

void GameSession::render(RenderWindow& window) {
    renderBackground(window);
    renderEntities(window);
    renderUI(window);
    if (debugHitboxes) {
        renderDebug(window);
    }
}

void GameSession::renderBackground(RenderWindow& window) {
    auto posPlayer = player->getPosition();
    background.drawTiled(window, posPlayer.x, posPlayer.y, screenSize.x, screenSize.y);
}

void GameSession::renderEntities(RenderWindow& window) {
    auto posPlayer = player->getPosition();
    float offsetX = screenSize.x / 2 - posPlayer.x;
    float offsetY = screenSize.y / 2 - posPlayer.y;
    player->draw(window, offsetX, offsetY);
    enemiesManager->draw(window, offsetX, offsetY);
    projectilesManager->draw(window, offsetX, offsetY);
}

void GameSession::renderUI(RenderWindow& window) {
    window.draw(positionText);
    window.draw(lifeText);

    string statsStr = "Currency: " + to_string(stats->getCurrency());
    Text statsText(statsStr, font, 18);
    statsText.setFillColor(Color::Cyan);
    statsText.setPosition(10, 40);
    window.draw(statsText);
}

void GameSession::renderDebug(RenderWindow& window) {
    debugHitboxesDisplay(
        window, *player, 
        screenSize.x / 2 - player->getPosition().x, 
        screenSize.y / 2 - player->getPosition().y, 
        enemiesManager.get(), 
        projectilesManager.get(), 
        player
    );
}

bool GameSession::isPlayerDead() const {
    return player->getLife() <= 0;
}

void GameSession::openShopMenu(RenderWindow& window) {
    if (shop) {
        shop->open(window, font);
    }
}

void GameSession::applyUpgrade(const Effect& effect) {
    if (player) {
        player->getStats().applyEffect(effect);
    }
}

void GameSession::enemyCollidedByProjectile(shared_ptr<Enemy> enemy, shared_ptr<Projectile> projectile) {
    if (enemy && projectile) {
        if(projectile->getCurrentEffectTrigger() == ProjectileEffects::PIERCING){
            enemy->getData().Life->takeDamage(projectile->getDamage());
        } 
        else {
            projectile->destroy();
        }
    }
}

void GameSession::drawDebugHitbox(RenderWindow& window, float x, float y, float radius, float offsetX, float offsetY) {
    CircleShape hitbox(radius/2);
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Blue);
    hitbox.setOutlineThickness(2.f);
    hitbox.setOrigin(radius/2, radius/2);
    hitbox.setPosition(x + offsetX, y + offsetY);
    window.draw(hitbox);

    CircleShape point(3);
    point.setFillColor(sf::Color::Yellow);
    point.setOrigin(3, 3);
    point.setPosition(x + offsetX, y + offsetY);
    window.draw(point);
}

void GameSession::drawDebugCapsule(RenderWindow& window, Vector2f pointA, Vector2f pointB, float radius, float offsetX, float offsetY){

    CircleShape bottom(radius);
    bottom.setOrigin(radius, radius);
    bottom.setPosition(pointA.x + offsetX, pointA.y + offsetY);
    bottom.setFillColor(Color::Transparent);
    bottom.setOutlineColor(Color::Blue);
    bottom.setOutlineThickness(2.f);
    window.draw(bottom);

    CircleShape top(radius);
    top.setOrigin(radius, radius);
    top.setPosition(pointB.x + offsetX, pointB.y + offsetY);
    top.setFillColor(Color::Transparent);
    top.setOutlineColor(Color::Blue);
    top.setOutlineThickness(2.f);
    window.draw(top);

    Vector2f dir = pointB - pointA;
    float length = sqrt(dir.x * dir.x + dir.y * dir.y);
    float angleDeg = atan2(dir.y, dir.x) * 180.0f / 3.14159265f;

    RectangleShape rect(Vector2f(length, radius * 2));
    rect.setOrigin(0, radius);
    rect.setPosition(pointA.x + offsetX, pointA.y + offsetY);
    rect.setRotation(angleDeg);
    rect.setFillColor(Color::Transparent);
    rect.setOutlineColor(Color::Green);
    rect.setOutlineThickness(2.f);
    window.draw(rect);

    Vertex line[] = {
        Vertex(Vector2f(pointA.x + offsetX, pointA.y + offsetY), Color::Yellow),
        Vertex(Vector2f(pointB.x + offsetX, pointB.y + offsetY), Color::Yellow)
    };
    window.draw(line, 2, sf::Lines);
}

void GameSession::debugHitboxesDisplay(RenderWindow& window, const Character& character, float offsetX, float offsetY, EnemiesManager* enemiesManager, ProjectilesManager* projectilesManager, const shared_ptr<Character>& player) {
    drawDebugHitbox(window, screenSize.x / 2, screenSize.y / 2, player->getCollisionBox().radius);

    for (const auto& enemy : enemiesManager->getEnemies()) {
        auto ebox = enemy->getCollisionBox();
        if (ebox.type == ShapeType::Circle) {
            drawDebugHitbox(window, ebox.center.x + offsetX, ebox.center.y + offsetY, ebox.radius);
        }
        else if (ebox.type == ShapeType::Capsule) {
            Vector2f pointA = ebox.center;
            float angleRad = ebox.rotationDeg * 3.14159265f / 180.0f;
            Vector2f axisDir(cos(angleRad), sin(angleRad));
            Vector2f pointB = pointA + axisDir * ebox.height;
            drawDebugCapsule(window, pointA, pointB, ebox.radius, offsetX, offsetY);        
        }
    }

    for (const auto& proj : projectilesManager->getProjectiles()) {
        auto ppos = proj->getPosition();
        drawDebugHitbox(window, ppos.x + offsetX, ppos.y + offsetY, proj->getSize());
    }
}