#include "Map/Map.h"

Map::Map(const Config& cfg) {
    g = MapGenerator::generate(cfg);
}