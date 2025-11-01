#ifndef SHOP_EVENTS_H
#define SHOP_EVENTS_H

#include "Shop/Shop.h"

namespace ShopEvents {
    void handleWindowEvents(Shop& shop, sf::RenderWindow& window, bool& running);
    void handleWindowClosed(Shop& shop, sf::RenderWindow& window, bool& running);
    void handleKeyPressed(Shop& shop, const sf::Event& event, bool& running);
}

#endif // SHOP_EVENTS_H