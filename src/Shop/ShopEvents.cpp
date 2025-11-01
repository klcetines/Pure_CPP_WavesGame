#include "Shop/ShopEvents.h"

using namespace sf;

void ShopEvents::handleWindowEvents(Shop& shop, RenderWindow& window, bool& running) {
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            handleWindowClosed(shop, window, running);
        }
        if (event.type == Event::KeyPressed) {
            handleKeyPressed(shop, event, running);
        }
    }
}

void ShopEvents::handleWindowClosed(Shop& shop, RenderWindow& window, bool& running) {
    window.close();
    running = false;
}

void ShopEvents::handleKeyPressed(Shop& shop, const Event& event, bool& running) {
    if (event.key.code == Keyboard::Left) {
        shop.selectPreviousItem();
    }
    if (event.key.code == Keyboard::Right) {
        shop.selectNextItem();
    }
    if (event.key.code == Keyboard::Escape || event.key.code == Keyboard::Space) {
        running = false;
    }
    if (event.key.code == Keyboard::Enter) {
        shop.purchaseSelectedItem();
    }
}