#include "GameSession.h"

GameSession::GameSession(Font& font)
    : font(font),
      background("assets/textures/cobblestone_1.png")
{
    player = make_shared<Character>("TouMate", 450, 350);
    enemiesManager = make_shared<EnemiesManager>();
    projectilesManager = make_shared<ProjectilesManager>();
    positionText = Text("Pos: (0, 0)", font, 20);
    positionText.setFillColor(Color::White);
    lifeText = Text("Life: 100", font, 20);
    lifeText.setFillColor(Color::Red);

    stats.enemiesKilled = 0;
    stats.projectilesFired = 0;
    stats.currency = 0;
}

void GameSession::update(float dt) {
    float speed = player->getData().Speed;
    if (Keyboard::isKeyPressed(Keyboard::W)) player->move(0, -speed);
    if (Keyboard::isKeyPressed(Keyboard::S)) player->move(0, speed);
    if (Keyboard::isKeyPressed(Keyboard::A)) player->move(-speed, 0);
    if (Keyboard::isKeyPressed(Keyboard::D)) player->move(speed, 0);

    auto posPlayer = player->getPosition();
    float offsetX = 900 / 2 - posPlayer.first;
    float offsetY = 700 / 2 - posPlayer.second;

    int before = enemiesManager->getEnemies().size();
    enemiesManager->update(dt, posPlayer);
    int after = enemiesManager->getEnemies().size();
    int killed = before - after;
    if (killed > 0) {
        stats.enemiesKilled += killed;
        stats.currency += killed; // currency = enemigos matados hasta que gastes
    }

    player->update(dt);
    player->handleCollisions(enemiesManager->getEnemies(), offsetX, offsetY);

    // Actualiza textos
    int life = static_cast<int>(player->getLife());
    lifeText.setString("Life: " + to_string(life));
    lifeText.setPosition(10, 10);

    positionText.setString("Pos: (" + to_string((int)posPlayer.first) + ", " + to_string((int)posPlayer.second) + ")");
    FloatRect textRect = positionText.getLocalBounds();
    positionText.setPosition(900 - textRect.width - 10, 700 - textRect.height - 10);

    // Disparo automático al enemigo más cercano
    auto closest = enemiesManager->getClosestEnemy(player->getPosition());
    if (closest) {
        auto proj = player->atack(closest->getPosition());
        if (proj) {
            projectilesManager->add(proj);
            stats.projectilesFired++;
        }
    }
    projectilesManager->update(dt);

    // Colisiones
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
}

bool GameSession::isPlayerDead() const {
    return player->getLife() <= 0;
}