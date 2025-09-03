#pragma once
#include "Shop/Shop.h"

namespace ShopRender {
    void render(Shop& shop, sf::RenderWindow& window, sf::Font& font);
    void renderTitle(Shop& shop, sf::RenderWindow& window, sf::Font& font);
    void renderShopItems(Shop& shop, sf::RenderWindow& window, sf::Font& font);
    void renderCard(sf::RenderWindow& window, const sf::Texture& texture, float xPos, float yPos, 
                 float width, float height, bool isSelected);
    void renderItemNumber(sf::RenderWindow& window, sf::Font& font, int number, float xPos, float yPos);
    void renderItemDescription(sf::RenderWindow& window, sf::Font& font, const std::string& description, 
                           float xPos, float yPos);

    string wrapText(const string& text, unsigned int maxLength);
    void renderItemName(sf::RenderWindow& window, sf::Font& font, const std::string& name, 
                     float xPos, float yPos, float cardWidth);
    void renderItemCost(sf::RenderWindow& window, sf::Font& font, int cost, 
                     float xPos, float yPos, float cardWidth);
    void renderCurrency(Shop& shop, sf::RenderWindow& window, sf::Font& font);


    static float currentOffset = 0.f;


}