#include "Rearrangement/RearrangeEvents.h"
#include "Rearrangement/Rearrange.h"

void RearrangeEvents::handleRearrangeInput(Rearrange& rearrange, sf::RenderWindow& window, bool& running) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            rearrange.closeRearrangeMenu();
        }
        if (event.type == sf::Event::KeyPressed) {
            handleKeyPressed(rearrange, event);
        }
    }
}

void RearrangeEvents::handleKeyPressed(Rearrange& rearrange, const sf::Event& event) {
    if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::O) {
        rearrange.closeRearrangeMenu();
    }
    if( event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Right) {
        rearrange.moveSelectionToNextEffect();
    }
    if( event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::Left) {
        rearrange.moveSelectionToPreviousEffect();
    }
    if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space) {
        rearrange.selectEffect();
    }
}