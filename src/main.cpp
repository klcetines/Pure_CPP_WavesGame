using namespace std;
#include <iostream>


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Mi primer juego con SFML");

    sf::CircleShape shape(50);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}