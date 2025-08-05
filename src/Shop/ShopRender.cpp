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

void ShopRender::renderShopItems(Shop& shop, RenderWindow& window, Font& font) {
    const auto& showcase = shop.getShowcaseItems();
    for (int i = 0; i < shop.getMaxShopItems(); ++i) {
        Text itemText(showcase[i].name + " (" + to_string(showcase[i].cost) + ")", font, 24);
        itemText.setPosition(50, 80 + i * 40);
        itemText.setFillColor(i == shop.getSelectedIndex() ? Color::Green : Color::White);
        window.draw(itemText);

        Text desc(showcase[i].description, font, 16);
        desc.setPosition(300, 85 + i * 40);
        desc.setFillColor(Color(180, 180, 180));
        window.draw(desc);
    }
}

void ShopRender::renderCurrency(Shop& shop, RenderWindow& window, Font& font) {
    Text currency("Currency: " + to_string(shop.getStats()->getCurrency()), font, 20);
    currency.setFillColor(Color::Cyan);
    currency.setPosition(30, 350);
    window.draw(currency);
}