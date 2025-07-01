#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <Character.h>
#include <Background.h>
#include <EnemiesManager.h>
#include <ProjectilesManager.h>
#include <PlayerProjectile.h>

using namespace std;
using namespace sf;

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 700;

void handleInput(Character& player) 
{
    float speed = 5.0f;
    if (Keyboard::isKeyPressed(Keyboard::W)) player.move(0, -speed);
    if (Keyboard::isKeyPressed(Keyboard::S)) player.move(0, speed);
    if (Keyboard::isKeyPressed(Keyboard::A)) player.move(-speed, 0);
    if (Keyboard::isKeyPressed(Keyboard::D)) player.move(speed, 0);
}
Font loadFont(const string& fontPath) {
    Font font;
    if (!font.loadFromFile(fontPath)) {
        cerr << "No se pudo cargar la fuente desde: " << fontPath << endl;
        exit(1);
    }
    return font;
}
Text instanciateText(const Font& font, const string& text, int size, Color color) 
{
    Text t;
    t.setFont(font);
    t.setString(text);
    t.setCharacterSize(size);
    t.setFillColor(color);
    return t;
}
void handleScreenText(Character& player, Text& positionText) 
{
    auto pos = player.getPosition();
    float x = pos.first;
    float y = pos.second;
    positionText.setString("Pos: (" + to_string((int)x) + ", " + to_string((int)y) + ")");
    FloatRect textRect = positionText.getLocalBounds();
    positionText.setPosition(SCREEN_WIDTH - textRect.width - 10, SCREEN_HEIGHT - textRect.height - 10);
}
void handleLifeText(Character& player, Text& lifeText) {
    int life = static_cast<int>(player.getLife());
    lifeText.setString("Life: " + to_string(life));
    // Puedes posicionarlo arriba a la izquierda, por ejemplo:
    lifeText.setPosition(10, 10);
}

int main() {
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "WaveControl 0.0.0");
    window.setFramerateLimit(60);

    Background background("assets/textures/cobblestone_1.png");
   
    shared_ptr<Character> player = make_shared<Character>("TouMate", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    shared_ptr<EnemiesManager> enemiesManager = make_shared<EnemiesManager>();
    shared_ptr<ProjectilesManager> projectilesManager = make_shared<ProjectilesManager>();

    Font font = loadFont("assets/fonts/circle.otf");
    Text positionText = instanciateText(font, "Pos: (0, 0)", 20, Color::White);
    Text lifeText = instanciateText(font, "Life: 100", 20, Color::Red);

    Clock clock;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();

        handleInput(*player);
        handleScreenText(*player, positionText);

        auto posPlayer = player->getPosition();
        float offsetX = SCREEN_WIDTH / 2 - posPlayer.first;
        float offsetY = SCREEN_HEIGHT / 2 - posPlayer.second;
        
        float dt = clock.restart().asSeconds();
        enemiesManager->update(dt, posPlayer);
        player->update(dt);

        player->handleCollisions(enemiesManager->getEnemies(), offsetX, offsetY);
        handleLifeText(*player, lifeText);

        auto closest = enemiesManager->getClosestEnemy(player->getPosition());
        if (closest) {
            auto proj = player->atack(closest->getPosition());
            if (proj) {
                projectilesManager->add(proj);
            }
        }

        window.clear();

        background.drawTiled(window, posPlayer.first, posPlayer.second, SCREEN_WIDTH, SCREEN_HEIGHT);
        player->draw(window);
        enemiesManager->draw(window, offsetX, offsetY);
        projectilesManager->update(dt);
        projectilesManager->draw(window, offsetX, offsetY);
        window.draw(positionText);
        window.draw(lifeText);
        window.display();
    }

    return 0;
}