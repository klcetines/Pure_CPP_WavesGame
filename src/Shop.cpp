#include "Shop.h"
#include "GameStatistics.h"
#include "GameSession.h"
#include <SFML/Window.hpp>

Shop::Shop(shared_ptr<GameStatistics> stats, GameSession& gameSession) : _stats(stats), _gameSession(gameSession) {
    loadItemsFromFile("assets/shop_items.csv");
}

void Shop::addItem(const ShopItem& item) {
    items.push_back(item);
}

void Shop::open(RenderWindow& window, Font& font) {
    bool running = true;
    while (running && window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                running = false;
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Up)
                    selectedIndex = (selectedIndex + items.size() - 1) % items.size();
                if (event.key.code == Keyboard::Down)
                    selectedIndex = (selectedIndex + 1) % items.size();
                if (event.key.code == Keyboard::Escape)
                    running = false;
                if (event.key.code == Keyboard::Enter) {
                    if (_stats->spendCurrency(items[selectedIndex].cost)) {
                        _gameSession.applyUpgrade(items[selectedIndex].effect);
                    }
                    else {
                        cout << "Not enough currency to buy " << items[selectedIndex].name << endl;
                    }
                }
            }
        }
        render(window, font);
        window.display();
        sleep(milliseconds(10));
    }
}

void Shop::render(RenderWindow& window, Font& font) {
    window.clear(Color(30, 30, 30));
    Text title("SHOP", font, 40);
    title.setFillColor(Color::Yellow);
    title.setPosition(30, 20);
    window.draw(title);

    for (size_t i = 0; i < items.size(); ++i) {
        Text itemText(items[i].name + " (" + to_string(items[i].cost) + ")", font, 24);
        itemText.setPosition(50, 80 + i * 40);
        itemText.setFillColor(i == selectedIndex ? Color::Green : Color::White);
        window.draw(itemText);

        Text desc(items[i].description, font, 16);
        desc.setPosition(300, 85 + i * 40);
        desc.setFillColor(Color(180, 180, 180));
        window.draw(desc);
    }

    Text currency("Currency: " + to_string(_stats->getCurrency()), font, 20);
    currency.setFillColor(Color::Cyan);
    currency.setPosition(30, 350);
    window.draw(currency);
}

void Shop::loadItemsFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo de items de la tienda: " << filename << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string name, costStr, description, effectType, effectValueStr;
        if (getline(ss, name, ',') &&
            getline(ss, costStr, ',') &&
            getline(ss, description, ',') &&
            getline(ss, effectType, ',') &&
            getline(ss, effectValueStr, ',')) {
            int cost = stoi(costStr);
            float effectValue = stof(effectValueStr);
            items.push_back({name, cost, description, Effect(effectType, effectValue)});
        }
    }
}