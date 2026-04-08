#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <cmath>
#include <string>
#include <algorithm>

#include "Utils/SelfRNG.h"

enum RoomType { START, END, BOSS, UPGRADE, SHOP, KEY, NORMAL };

struct Node {
    int id;
    RoomType type;
    bool hasKey;
    std::vector<int> children;
    int parent;
    int depth;
    double gx, gy; // graph layout
    int mx, my;    // map layout
    int dist;      // distance from start
};

struct Edge {
    int id;
    int from, to;
    bool isLoop;
};

struct Config {
    int seed;
    int normalRooms;
    int loopDensity;
    int branchFactor;
};

struct Graph {
    std::vector<Node> nodes;
    std::vector<Edge> edges;
    int startId;
    int endId;
    struct Zones { int earlyEnd, midEnd, maxDepth; } zones;
    struct Stats { int rooms, edges, loops, earlyEnd, midEnd, maxDepth; } stats;
};
 
class MapGenerator {
public:
    static Graph generate(const Config& cfg);

private:
    static void createInitialNodes(Graph& g, int totalRooms);
    static void buildTree(Graph& g, ImprovedRNG& rng, int branchFactor);
    static void calculateDepthsAndZones(Graph& g);
    static void assignSpecialRooms(Graph& g, ImprovedRNG& rng);
    static void buildEdgesAndLoops(Graph& g, ImprovedRNG& rng, int loopDensity);
    static void calculateDistances(Graph& g);
    static void calculateGraphLayout(Graph& g, ImprovedRNG& rng);
    static void calculateMapLayout(Graph& g, ImprovedRNG& rng);
    static void calculateStats(Graph& g);
};

#endif // MAP_GENERATOR_H