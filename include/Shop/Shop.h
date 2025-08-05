#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include "GameStatistics.h"
#include "Effect.h"

using namespace sf;
using namespace std;

class GameSession;

struct ShopItem {
    string name;
    int cost;
    string description;
    list<Effect> effects;
};

class Shop {
public:
    Shop(shared_ptr<GameStatistics> stats, GameSession& gameSession);
    void open(RenderWindow& window, Font& font);
    void addItem(const ShopItem& item);

    const vector<ShopItem>& getShowcaseItems() const;
    int getSelectedIndex() const;
    int getMaxShopItems() const;
    shared_ptr<GameStatistics> getStats() const;

    void selectPreviousItem();
    void selectNextItem();
    void purchaseSelectedItem();

private:
    shared_ptr<GameStatistics> _stats;
    GameSession& _gameSession;
    vector<ShopItem> items;
    vector<ShopItem> _showcaseItems;
    int selectedIndex = 0;
    int _maxShopItems = 4;

    void loadItemsFromFile(const string& filename);
    void reportFileOpenError(const string& filename);
    bool parseItem(const string& line, ShopItem& item);
    void parseItemEffects(const string& effectStr, list<Effect>& effects);
    bool parseShopItemLine(const string& line, ShopItem& item);
    list<Effect> parseEffects(const string& effectStr);
    bool parseEffect(const string& changeStr, Effect& effect);
    void assignRandomItemToShowcase(int index);
    void setShowcaseItemSoldOut(int index);
    void restockShowcaseItems();
    void restockShowcaseSingleItem(int index);
    void applySelectedItemEffects();
};