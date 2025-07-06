#include "GameSession.h"

GameSession::GameSession(Font& font)
    : font(font),
      background("assets/textures/cobblestone_1.png")
{
    stats = make_shared<GameStatistics>();
    player = make_shared<Character>("TouMate", 450, 350);
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
    float offsetX = 900 / 2 - posPlayer.first;
    float offsetY = 700 / 2 - posPlayer.second;

    enemiesManager->update(dt, posPlayer);

    player->update(dt);
    player->handleCollisions(enemiesManager->getEnemies(), offsetX, offsetY);

    // Actualiza textos
    int life = static_cast<int>(player->getLife());
    lifeText.setString("Life: " + to_string(life));
    lifeText.setPosition(10, 10);

    positionText.setString("Pos: (" + to_string((int)posPlayer.first) + ", " + to_string((int)posPlayer.second) + ")");
    FloatRect textRect = positionText.getLocalBounds();
    positionText.setPosition(900 - textRect.width - 10, 700 - textRect.height - 10);

    auto closest = enemiesManager->getClosestEnemy(player->getPosition());
    if (closest) {
        auto proj = player->atack(closest->getPosition());
        if (proj) {
            projectilesManager->add(proj);
            stats->addProjectile();
        }
    }
    projectilesManager->update(dt);

    auto& projectiles = projectilesManager->getProjectiles();
    auto& enemies = enemiesManager->getEnemies();
    for (auto& enemy : enemies) {
        for (auto& proj : projectiles) {
            auto epos = enemy->getPosition();
            auto ppos = proj->getPosition();
            float dist = std::sqrt(
                (epos.first - ppos.x) * (epos.first - ppos.x) +
                (epos.second - ppos.y) * (epos.second - ppos.y)
            );
            if (dist < enemy->getSize() + 6) {
                enemy->getData().Life->takeDamage(proj->getDamage());
                proj->destroy();
            }
        }
    }
}

void GameSession::render(RenderWindow& window) {
    auto posPlayer = player->getPosition();
    float offsetX = 900 / 2 - posPlayer.first;
    float offsetY = 700 / 2 - posPlayer.second;

    background.drawTiled(window, posPlayer.first, posPlayer.second, 900, 700);
    player->draw(window);
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