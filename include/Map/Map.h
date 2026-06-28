#ifndef MAP_H
#define MAP_H

#include "Map/MapGenerator.h"
#include "Map/MapRenderer.h"

class Map {
    public:
        Graph g;
        MapRenderer* mapRenderer;
        Map(const Config& cfg, float playerSize, const sf::Vector2u& windowSize);
        void update(float dt, const sf::Vector2f& playerWorldPos);
        void draw(sf::RenderWindow& window, sf::Vector2f cameraPos, bool debugGrid = false) const;
};

#endif // MAP_H