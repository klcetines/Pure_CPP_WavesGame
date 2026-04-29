#include "Map/Map.h"

Map::Map(const Config& cfg, float playerSize, const Vector2u& windowSize) {
    g = MapGenerator::generate(cfg);
    mapRenderer = new MapRenderer();
    mapRenderer->build(g, playerSize, windowSize);
}

void Map::update(float dt, const Vector2f& playerWorldPos) {
    mapRenderer->update(dt, playerWorldPos);
}

void Map::draw(RenderWindow& window, Vector2f cameraPos, bool debugGrid) const {
    mapRenderer->draw(window, cameraPos, debugGrid);
    mapRenderer->drawMinimap(window, cameraPos);
}