#ifndef MAP_H
#define MAP_H

#include "Map/MapGenerator.h"
#include "Map/MapRenderer.h"

class Map {
    public:
        Graph g;
        MapRenderer* mapRenderer;
        Map(const Config& cfg, float playerSize, const Vector2u& windowSize);
        void update(float dt, const Vector2f& playerWorldPos);
        void draw(RenderWindow& window, Vector2f cameraPos, bool debugGrid = false) const;
};

#endif // MAP_H