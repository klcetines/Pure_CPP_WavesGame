#ifndef SHOP_H
#define SHOP_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include "GameManagers/GameStatistics.h"
#include "Utils/Effect.h"


class GameSession;

struct ShopItem {
    std::string name;
    int cost;
    std::string description;
    std::list<Effect> effects;
};

class Shop {
public:
    Shop(std::shared_ptr<GameStatistics> stats, GameSession& gameSession);
    void open(sf::RenderWindow& window, sf::Font& font);
    void addItem(const ShopItem& item);

    const std::vector<ShopItem>& getShowcaseItems() const;
    int getSelectedIndex() const;
    int getMaxShopItems() const;
    std::shared_ptr<GameStatistics> getStats() const;

    void selectPreviousItem();
    void selectNextItem();
    void purchaseSelectedItem();

private:
    std::shared_ptr<GameStatistics> _stats;
    GameSession& _gameSession;
    std::vector<ShopItem> items;
    std::vector<ShopItem> _showcaseItems;
    int selectedIndex = 0;
    int _maxShopItems = 4;

    void loadItemsFromFile(const std::string& filename);
    void reportFileOpenError(const std::string& filename);
    bool parseItem(const std::string& line, ShopItem& item);
    void parseItemEffects(const std::string& effectStr, std::list<Effect>& effects);
    bool parseShopItemLine(const std::string& line, ShopItem& item);
    std::list<Effect> parseEffects(const std::string& effectStr);
    bool parseEffect(const std::string& changeStr, Effect& effect);
    void assignRandomItemToShowcase(int index);
    void setShowcaseItemSoldOut(int index);
    void restockShowcaseItems();
    void restockShowcaseSingleItem(int index);
    void applySelectedItemEffects();
};

#endif // SHOP_H