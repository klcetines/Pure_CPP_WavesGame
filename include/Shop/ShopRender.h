#pragma once
#include "Shop/Shop.h"

namespace ShopRender {
    void render(Shop& shop, sf::RenderWindow& window, sf::Font& font);
    void renderTitle(Shop& shop, sf::RenderWindow& window, sf::Font& font);
    void renderShopItems(Shop& shop, sf::RenderWindow& window, sf::Font& font);
    void renderCurrency(Shop& shop, sf::RenderWindow& window, sf::Font& font);
}