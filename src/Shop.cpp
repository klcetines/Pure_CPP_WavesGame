#include "Shop.h"
#include "GameStatistics.h"
#include "GameSession.h"
#include <SFML/Window.hpp>

Shop::Shop(shared_ptr<GameStatistics> stats, GameSession& gameSession) : _stats(stats), _gameSession(gameSession) {
    loadItemsFromFile("assets/shop_items.csv");
    _maxShopItems = min(static_cast<int>(items.size()), 4);
    _showcaseItems.resize(_maxShopItems);
    restockShowcaseItems();
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
                    selectedIndex = (selectedIndex + _showcaseItems.size() - 1) % _showcaseItems.size();
                if (event.key.code == Keyboard::Down)
                    selectedIndex = (selectedIndex + 1) % _showcaseItems.size();
                if (event.key.code == Keyboard::Escape || event.key.code == Keyboard::Space)
                    running = false;
                if (event.key.code == Keyboard::Enter) {
                    if (_stats->spendCurrency(_showcaseItems[selectedIndex].cost)) {
                        cout << _showcaseItems[selectedIndex].effects.size()<< endl;
                        for(const auto& effect : _showcaseItems[selectedIndex].effects) {
                            _gameSession.applyUpgrade(effect);
                        }
                        restockShowcaseSingleItem(selectedIndex);
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

    for (int i = 0; i < _maxShopItems; ++i) {
        Text itemText(_showcaseItems[i].name + " (" + to_string(_showcaseItems[i].cost) + ")", font, 24);
        itemText.setPosition(50, 80 + i * 40);
        itemText.setFillColor(i == selectedIndex ? Color::Green : Color::White);
        window.draw(itemText);

        Text desc(_showcaseItems[i].description, font, 16);
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
        string name, costStr, description, effectType, effectStr;
        if (getline(ss, name, ',') &&
            getline(ss, costStr, ',') &&
            getline(ss, description, ',') &&
            getline(ss, effectStr, ',')
        ){        
            int cost = stoi(costStr);
            list<Effect>* effects = new list<Effect>();
            stringstream effectStream(effectStr);
            string changeStr;
            while (getline(effectStream, changeStr, ';')) {
                size_t sep = changeStr.find(':');
                if (sep != string::npos) {
                    string type = changeStr.substr(0, sep);
                    float value = stof(changeStr.substr(sep + 1));
                    effects->push_back(Effect(type, value));
                }
            }
            items.push_back({name, cost, description, *effects});
        }
    }
}

void Shop::restockShowcaseItems(){
    for (int i = 0; i < _maxShopItems; ++i) {
        restockShowcaseSingleItem(i);
    }
}

void Shop::restockShowcaseSingleItem(int index) {
    if (index < 0 || index >= _maxShopItems) return;
    if (items.empty()){
        _showcaseItems[index] = {"EMPTY", 0, "Sold Out!", list<Effect>()};
    } 
    else{
        int newIndex = rand() % items.size();
        _showcaseItems[index] = items[newIndex];
        items.erase(items.begin() + newIndex);
    }
}