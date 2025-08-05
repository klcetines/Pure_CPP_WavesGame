#include "Shop/Shop.h"
#include "Shop/ShopRender.h"
#include "Shop/ShopEvents.h"
#include "GameStatistics.h"
#include "GameSession.h"
#include <SFML/Window.hpp>

Shop::Shop(shared_ptr<GameStatistics> stats, GameSession& gameSession) : _stats(stats), _gameSession(gameSession) {
    loadItemsFromFile("assets/shop_items.csv");
    _maxShopItems = min(static_cast<int>(items.size()), 4);
    _showcaseItems.resize(_maxShopItems);
    restockShowcaseItems();
}

void Shop::open(RenderWindow& window, Font& font) {
    bool running = true;
    while (running && window.isOpen()) {
        ShopEvents::handleWindowEvents(*this, window, running);
        ShopRender::render(*this, window, font);
        window.display();
        sleep(milliseconds(10));
    }
}

void Shop::addItem(const ShopItem& item) {
    items.push_back(item);
}

const vector<ShopItem>& Shop::getShowcaseItems() const {
    return _showcaseItems;
}

int Shop::getSelectedIndex() const {
    return selectedIndex;
}

int Shop::getMaxShopItems() const {
    return _maxShopItems;
}

shared_ptr<GameStatistics> Shop::getStats() const {
    return _stats;
}

void Shop::selectPreviousItem() {
    if (_maxShopItems == 0) return;
    selectedIndex = (selectedIndex + _maxShopItems - 1) % _maxShopItems;
}

void Shop::selectNextItem() {
    if (_maxShopItems == 0) return;
    selectedIndex = (selectedIndex + 1) % _maxShopItems;
}

void Shop::purchaseSelectedItem() {
    if (_maxShopItems == 0) return;
    const ShopItem& item = _showcaseItems[selectedIndex];
    if (_stats->spendCurrency(item.cost)) {
        applySelectedItemEffects();
        restockShowcaseSingleItem(selectedIndex);
    }
}

void Shop::loadItemsFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        reportFileOpenError(filename);
        return;
    }
    string line;
    while (getline(file, line)) {
        ShopItem item;
        if (parseShopItemLine(line, item)) {
            items.push_back(item);
        }
    }
}

void Shop::reportFileOpenError(const string& filename) {
    cerr << "No se pudo abrir el archivo de items de la tienda: " << filename << endl;
}

bool Shop::parseShopItemLine(const string& line, ShopItem& item) {
    stringstream ss(line);
    string name, costStr, description, effectStr;
    if (getline(ss, name, ',') &&
        getline(ss, costStr, ',') &&
        getline(ss, description, ',') &&
        getline(ss, effectStr, ',')) {
        item.name = name;
        item.cost = stoi(costStr);
        item.description = description;
        item.effects = parseEffects(effectStr);
        return true;
    }
    return false;
}

list<Effect> Shop::parseEffects(const string& effectStr) {
    list<Effect> effects;
    stringstream effectStream(effectStr);
    string changeStr;
    while (getline(effectStream, changeStr, ';')) {
        Effect effect;
        if (parseEffect(changeStr, effect)) {
            effects.push_back(effect);
        }
    }
    return effects;
}

bool Shop::parseEffect(const string& changeStr, Effect& effect) {
    size_t sep = changeStr.find(':');
    if (sep != string::npos) {
        string type = changeStr.substr(0, sep);
        float value = stof(changeStr.substr(sep + 1));
        effect = Effect(type, value);
        return true;
    }
    return false;
}

void Shop::restockShowcaseItems() {
    for (int i = 0; i < _maxShopItems; ++i) {
        restockShowcaseSingleItem(i);
    }
}

void Shop::restockShowcaseSingleItem(int index) {
    if (index < 0 || index >= _maxShopItems) return;
    if (items.empty()) {
        setShowcaseItemSoldOut(index);
    } else {
        assignRandomItemToShowcase(index);
    }
}

void Shop::setShowcaseItemSoldOut(int index) {
    _showcaseItems[index] = {"EMPTY", 0, "Sold Out!", list<Effect>()};
}

void Shop::assignRandomItemToShowcase(int index) {
    int newIndex = rand() % items.size();
    _showcaseItems[index] = items[newIndex];
    items.erase(items.begin() + newIndex);
}

void Shop::applySelectedItemEffects() {
    const ShopItem& item = _showcaseItems[selectedIndex];
    for (const auto& effect : item.effects) {
        _gameSession.applyUpgrade(effect);
    }
}


