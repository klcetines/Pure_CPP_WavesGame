#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <Character.h>
#include <Background.h>
#include <EnemiesManager.h>
#include <ProjectilesManager.h>
#include <PlayerProjectile.h>
#include <GameState.h>
#include <GameSession.h>

using namespace std;
using namespace sf;

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 700;

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

void processEvents(RenderWindow& window) {
    Event event;
    while (window.pollEvent(event))
        if (event.type == Event::Closed)
            window.close();
}

void showMainMenu(RenderWindow& window, Font& font, GameState& gameState) {
    Text title("TU JUEGO", font, 60);
    title.setFillColor(Color::White);
    title.setPosition(SCREEN_WIDTH/2 - title.getLocalBounds().width/2, 150);

    Text start("Pulsa ENTER para jugar", font, 30);
    start.setFillColor(Color::Green);
    start.setPosition(SCREEN_WIDTH/2 - start.getLocalBounds().width/2, 300);

    window.clear(Color::Black);
    window.draw(title);
    window.draw(start);
    window.display();

    while (window.isOpen() && gameState.isMainMenu()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
                gameState.setState(GameStateType::Playing);
        }
        sf::sleep(sf::milliseconds(10));
    }
}

void showGameOverMenu(RenderWindow& window, Font& font, GameState& gameState) {
    Text over("GAME OVER", font, 60);
    over.setFillColor(Color::Red);
    over.setPosition(SCREEN_WIDTH/2 - over.getLocalBounds().width/2, 150);

    Text retry("Pulsa R para reiniciar o ESC para salir", font, 30);
    retry.setFillColor(Color::White);
    retry.setPosition(SCREEN_WIDTH/2 - retry.getLocalBounds().width/2, 300);

    window.clear(Color::Black);
    window.draw(over);
    window.draw(retry);
    window.display();

    while (window.isOpen() && gameState.isGameOver()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::R)
                    gameState.setState(GameStateType::Playing);
                if (event.key.code == Keyboard::Escape)
                    window.close();
            }
        }
        sf::sleep(sf::milliseconds(10));
    }
}

int main() {
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "WaveControl 0.0.0");
    window.setFramerateLimit(60);

    Font font = loadFont("assets/fonts/circle.otf");
    GameState gameState;

    while (window.isOpen()) {
        if (gameState.isMainMenu()) {
            showMainMenu(window, font, gameState);
        }

        GameSession session(font, window.getSize());
        Clock clock;

        while (window.isOpen() && gameState.isPlaying()) {
            processEvents(window);

            float dt = clock.restart().asSeconds();
            session.update(dt, window);
            window.clear();
            session.render(window);
            window.display();

            if (session.isPlayerDead()) {
                gameState.setState(GameStateType::GameOver);
            }
        }

        if (gameState.isGameOver()) {
            showGameOverMenu(window, font, gameState);
        }
    }
    return 0;
}