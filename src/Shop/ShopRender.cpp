#include "Shop/ShopRender.h"

using namespace sf;

void ShopRender::render(Shop& shop, RenderWindow& window, Font& font) {
    window.clear(Color(30, 30, 30));
    renderTitle(shop, window, font);
    renderShopItems(shop, window, font);
    renderCurrency(shop, window, font);
}

void ShopRender::renderTitle(Shop& shop, RenderWindow& window, Font& font) {
    Text title("SHOP", font, 40);
    title.setFillColor(Color::Yellow);
    title.setPosition(30, 20);
    window.draw(title);
}

void ShopRender::renderCard(RenderWindow& window, const Texture& texture, float xPos, float yPos, 
                         float width, float height, bool isSelected) {
    Sprite cardSprite(texture);
    cardSprite.setPosition(xPos, yPos);
    cardSprite.setScale(
        width / cardSprite.getLocalBounds().width,
        height / cardSprite.getLocalBounds().height
    );
    cardSprite.setColor(isSelected ? Color(150, 255, 150) : Color::White);
    window.draw(cardSprite);
}

void ShopRender::renderShopItems(Shop& shop, RenderWindow& window, Font& font) {
    const auto& showcase = shop.getShowcaseItems();
    Texture cardTexture;
    if (!cardTexture.loadFromFile("assets/sprites/Cards/atac.png")) {
        return;
    }

    const float cardWidth = 250.f;
    const float cardHeight = 350.f;
    const float cardSpacing = 30.f;
    const pair<float, float> windowSize = { 
            static_cast<float>(window.getSize().x),
            static_cast<float>(window.getSize().y) 
        };

    const float startY = (windowSize.first - cardHeight) / 2.f;

    const int selectedIndex = shop.getSelectedIndex();
    const float targetOffset = -(selectedIndex * (cardWidth + cardSpacing)) + (windowSize.second / 2.f - cardWidth / 2.f);

    const float lerpFactor = 0.1f;
    currentOffset = currentOffset + (targetOffset - currentOffset) * lerpFactor;

    for (int i = 0; i < shop.getMaxShopItems(); ++i) {
        float xPos = (cardWidth + cardSpacing) * i + currentOffset;
        
        if (xPos + cardWidth < -100 || xPos > windowSize.second + 100) {
            continue;
        }

        renderCard(window, cardTexture, xPos, startY, cardWidth, cardHeight, i == selectedIndex);
        renderItemName(window, font, showcase[i].name, xPos, startY + 70, cardWidth);
        renderItemCost(window, font, showcase[i].cost, xPos + 55, startY + 25, cardWidth);
        renderItemDescription(window, font, showcase[i].description, xPos, startY + 255);
    }
}

void ShopRender::renderItemDescription(RenderWindow& window, Font& font, const string& description, 
                                   float xPos, float yPos) {
    string wrappedText = wrapText(description, 30);
    
    Text desc(wrappedText, font, 15);  
    desc.setPosition(xPos + 20, yPos);
    desc.setScale(0.9f, 0.9f);
    desc.setFillColor(Color::Black);
    window.draw(desc);
}

string ShopRender::wrapText(const string& text, unsigned int maxLength) {
    string result;
    string currentLine;
    string currentWord;

    for (char c : text) {
        if (c == ' ') {
            if (currentLine.length() + currentWord.length() > maxLength && !currentLine.empty()) {
                result += currentLine + '\n';
                currentLine = currentWord + ' ';
                currentWord = "";
            } else {
                currentLine += currentWord + ' ';
                currentWord = "";
            }
        } else {
            currentWord += c;
        }
    }

    // Handle the last word
    if (currentLine.length() + currentWord.length() > maxLength && !currentLine.empty()) {
        result += currentLine + '\n' + currentWord;
    } else {
        result += currentLine + currentWord;
    }

    return result;
}

void ShopRender::renderItemName(RenderWindow& window, Font& font, const string& name, 
                            float xPos, float yPos, float cardWidth) {
    Text itemText(name, font, 24);
    float textX = xPos + (cardWidth - itemText.getLocalBounds().width) / 2;
    itemText.setPosition(textX, yPos);
    itemText.setFillColor(Color::Black);
    window.draw(itemText);
}

void ShopRender::renderItemCost(RenderWindow& window, Font& font, int cost, 
                            float xPos, float yPos, float cardWidth) {
    Text costText(to_string(cost), font, 22);
    float textX = xPos + (cardWidth - costText.getLocalBounds().width) / 2;
    costText.setPosition(textX, yPos);
    costText.setFillColor(Color::Black);
    window.draw(costText);
}

void ShopRender::renderCurrency(Shop& shop, RenderWindow& window, Font& font) {
    Text currency("Currency: " + to_string(shop.getStats()->getCurrency()), font, 20);
    currency.setFillColor(Color::Cyan);
    currency.setPosition(30, 350);
    window.draw(currency);
}