#ifndef MAP_H
#define MAP_H

#include "Map/MapGenerator.h"

class Map {
    public:
        Graph g;
        Map(const Config& cfg);
};

#endif // MAP_H