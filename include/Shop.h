#ifndef SHOP_H
#define SHOP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "GameStatistics.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include "Effect.h"

using namespace sf;
using namespace std;

class GameSession;

struct ShopItem {
    string name;
    int cost;
    string description;
    Effect effect;
};

class Shop {
public:
    Shop(shared_ptr<GameStatistics> stats, GameSession& gameSession);

    void open(RenderWindow& window, Font& font);
    void addItem(const ShopItem& item);

private:
    shared_ptr<GameStatistics> _stats;
    GameSession& _gameSession;
    vector<ShopItem> items;
    vector<ShopItem> _showcaseItems;
    int selectedIndex = 0;
    int _maxShopItems = 4;

    void render(RenderWindow& window, Font& font);
    void handleInput(bool& running);
    void loadItemsFromFile(const string& filename);
    void restockShowcaseItems();
    void restockShowcaseSingleItem(int index);
};

#endif