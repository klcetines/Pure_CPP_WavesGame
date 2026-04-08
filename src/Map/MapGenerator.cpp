#include "Map/MapGenerator.h"
#include <tuple> // Necesario para std::tuple

MapGenerator::Graph MapGenerator::generate(const Config& cfg) {
    SelfRNG rng(cfg.seed);
    Graph g;

    const int SPECIAL_ROOMS = 6;
    int totalRooms = cfg.normalRooms + SPECIAL_ROOMS + 2;

    createInitialNodes(g, totalRooms);
    buildTree(g, rng, cfg.branchFactor);
    calculateDepthsAndZones(g);
    assignSpecialRooms(g, rng);
    buildEdgesAndLoops(g, rng, cfg.loopDensity);
    calculateDistances(g);
    calculateGraphLayout(g, rng);
    calculateMapLayout(g, rng);
    calculateStats(g);

    return g;
}

void MapGenerator::createInitialNodes(Graph& g, int totalRooms) {
    auto mkNode = [&](RoomType type) -> int {
        int id = g.nodes.size();
        g.nodes.push_back({id, type, false, {}, -1, 0, 0.0, 0.0, 0, 0, 0});
        return id;
    };

    g.startId = mkNode(START);
    for (int i = 0; i < totalRooms - 1; ++i) {
        mkNode(NORMAL);
    }
}

void MapGenerator::buildTree(Graph& g, ImprovedRNG& rng, int branchFactor) {
    std::vector<int> pool;
    for (size_t i = 1; i < g.nodes.size(); ++i) {
        pool.push_back(g.nodes[i].id);
    }

    std::vector<int> frontier = {g.startId};
    size_t pi = 0;
    while (pi < pool.size() && !frontier.empty()) {
        bool useBranch = rng.f() < (branchFactor / 100.0);
        int fi = useBranch ? rng.i(0, frontier.size() - 1) : frontier.size() - 1;
        
        int parentId = frontier[fi];
        int childId = pool[pi++];
        
        g.nodes[childId].parent = parentId;
        g.nodes[childId].depth = g.nodes[parentId].depth + 1;
        g.nodes[parentId].children.push_back(childId);
        
        frontier.push_back(childId);
    }
}

void MapGenerator::calculateDepthsAndZones(Graph& g) {
    int maxDepth = 0;
    for (const auto& n : g.nodes) {
        maxDepth = std::max(maxDepth, n.depth);
    }
    int earlyEnd = std::max(2, static_cast<int>(maxDepth * 0.30));
    int midEnd = std::max(earlyEnd + 2, static_cast<int>(maxDepth * 0.62));
    
    g.zones = {earlyEnd, midEnd, maxDepth};
}

void MapGenerator::assignSpecialRooms(Graph& g, ImprovedRNG& rng) {
    auto nodesInZone = [&](int dMin, int dMax, const std::set<int>& exclude) {
        std::vector<Node*> res;
        for (auto& n : g.nodes) {
            if (n.id != g.startId && n.depth >= dMin && n.depth <= dMax && exclude.find(n.id) == exclude.end()) {
                res.push_back(&n);
            }
        }
        return res;
    };

    std::set<int> assigned = {g.startId};

    auto assignSpecial = [&](RoomType type, std::vector<Node*> candidates) -> int {
        if (candidates.empty()) return -1;
        Node* n = rng.pick(candidates);
        n->type = type;
        if (type == KEY) n->hasKey = true;
        assigned.insert(n->id);
        return n->id;
    };

    assignSpecial(SHOP, nodesInZone(2, g.zones.midEnd, assigned));
    int upgradeId = assignSpecial(UPGRADE, nodesInZone(g.zones.earlyEnd + 1, g.zones.midEnd, assigned));
    
    int upgradeDepth = (upgradeId >= 0) ? g.nodes[upgradeId].depth : g.zones.earlyEnd;
    auto bossCandidates = nodesInZone(g.zones.midEnd + 1, g.zones.maxDepth - 1, assigned);
    
    std::vector<Node*> bossPool;
    for (auto* n : bossCandidates) {
        if (n->parent != g.startId && n->depth > upgradeDepth) bossPool.push_back(n);
    }
    
    if (bossPool.empty()) {
        bossPool = nodesInZone(g.zones.earlyEnd + 2, g.zones.maxDepth - 1, assigned);
        std::vector<Node*> filtered;
        for (auto* n : bossPool) {
            if (n->parent != g.startId && n->depth > upgradeDepth) filtered.push_back(n);
        }
        bossPool = filtered;
    }
    assignSpecial(BOSS, bossPool);

    assignSpecial(KEY, nodesInZone(1, g.zones.earlyEnd, assigned));
    assignSpecial(KEY, nodesInZone(g.zones.earlyEnd, g.zones.midEnd, assigned));
    assignSpecial(KEY, nodesInZone(g.zones.midEnd, g.zones.maxDepth, assigned));

    std::vector<Node*> leaves;
    for (auto& n : g.nodes) {
        if (n.children.empty() && assigned.find(n.id) == assigned.end()) leaves.push_back(&n);
    }
    std::sort(leaves.begin(), leaves.end(), [](Node* a, Node* b) { return a->depth > b->depth; });
    
    Node* endNode = leaves.empty() ? nullptr : leaves[0];
    if (!endNode) {
        std::vector<Node*> normals;
        for (auto& n : g.nodes) {
            if (assigned.find(n.id) == assigned.end()) normals.push_back(&n);
        }
        std::sort(normals.begin(), normals.end(), [](Node* a, Node* b) { return a->depth > b->depth; });
        endNode = normals.empty() ? nullptr : normals[0];
    }
    
    if (endNode) {
        endNode->type = END;
        endNode->hasKey = false;
        assigned.insert(endNode->id);
        g.endId = endNode->id;
    } else {
        g.endId = -1;
    }

    for (auto& n : g.nodes) {
        if (n.type == BOSS) n.hasKey = true;
    }
}

void MapGenerator::buildEdgesAndLoops(Graph& g, ImprovedRNG& rng, int loopDensity) {
    std::set<std::string> eSet;
    auto addEdge = [&](int a, int b, bool isLoop = false) -> int {
        if (a == b) return -1;
        std::string k = std::to_string(std::min(a, b)) + "," + std::to_string(std::max(a, b));
        if (eSet.count(k)) return -1;
        eSet.insert(k);
        int eid = g.edges.size();
        g.edges.push_back({eid, a, b, isLoop});
        return eid;
    };

    for (const auto& n : g.nodes) {
        if (n.parent >= 0) addEdge(n.parent, n.id, false);
    }

    auto treeDistBFS = [&](int a, int b) -> int {
        std::vector<bool> visited(g.nodes.size(), false);
        std::queue<std::pair<int, int>> q;
        q.push({a, 0});
        visited[a] = true;
        while (!q.empty()) {
            auto [cur, d] = q.front(); q.pop();
            if (cur == b) return d;
            
            std::vector<int> nbrs = {g.nodes[cur].parent};
            nbrs.insert(nbrs.end(), g.nodes[cur].children.begin(), g.nodes[cur].children.end());
            
            for (int nb : nbrs) {
                if (nb >= 0 && !visited[nb]) {
                    visited[nb] = true;
                    q.push({nb, d + 1});
                }
            }
        }
        return 999;
    };

    int loopTarget = static_cast<int>(g.nodes.size() * (loopDensity / 100.0) * 0.5);
    int loopCount = 0;
    std::vector<std::tuple<int, int, int>> candidates;
    
    for (size_t i = 0; i < g.nodes.size(); ++i) {
        for (size_t j = i + 1; j < g.nodes.size(); ++j) {
            int td = treeDistBFS(i, j);
            if (td >= 3 && td <= 6) {
                candidates.push_back({static_cast<int>(i), static_cast<int>(j), td});
            }
        }
    }
    
    rng.sh(candidates);
    for (auto [a, b, _] : candidates) {
        if (loopCount >= loopTarget) break;
        if (addEdge(a, b, true) >= 0) ++loopCount;
    }
}

void MapGenerator::calculateDistances(Graph& g) {
    std::vector<int> dist(g.nodes.size(), 999);
    dist[g.startId] = 0;
    std::queue<int> bfsQ;
    bfsQ.push(g.startId);
    
    while (!bfsQ.empty()) {
        int cur = bfsQ.front(); 
        bfsQ.pop();
        for (const auto& e : g.edges) {
            int nb = -1;
            if (e.from == cur) nb = e.to;
            else if (e.to == cur) nb = e.from;
            
            if (nb >= 0 && dist[nb] == 999) {
                dist[nb] = dist[cur] + 1;
                bfsQ.push(nb);
            }
        }
    }
    for (auto& n : g.nodes) n.dist = dist[n.id];
}

void MapGenerator::calculateGraphLayout(Graph& g, ImprovedRNG& rng) {
    std::map<int, std::vector<int>> byDist;
    for (const auto& n : g.nodes) byDist[n.dist].push_back(n.id);
    
    double LX = 140.0, LY = 80.0;
    for (auto& [d, ids] : byDist) {
        double total = (ids.size() - 1) * LY;
        for (size_t i = 0; i < ids.size(); ++i) {
            double jx = rng.f() * 30 - 15;
            double jy = rng.f() * 20 - 10;
            g.nodes[ids[i]].gx = d * LX + jx;
            g.nodes[ids[i]].gy = -total / 2 + i * LY + jy;
        }
    }
}

void MapGenerator::calculateMapLayout(Graph& g, ImprovedRNG& rng) {
    std::map<std::string, int> grid;
    auto gridKey = [](int x, int y) { return std::to_string(x) + "," + std::to_string(y); };
    
    auto freeAround = [&](int x, int y) -> std::pair<int, int> {
        std::vector<std::pair<int, int>> dirs = {{1,0},{0,1},{0,-1},{-1,0}};
        for (auto [dx, dy] : dirs) {
            if (grid.find(gridKey(x + dx, y + dy)) == grid.end()) return {x + dx, y + dy};
        }
        // Si no hay hueco, empujamos lejos para evitar colisión
        return {x + 10 + rng.i(0, 5), y + rng.i(-3, 3)};
    };

    g.nodes[g.startId].mx = 0; 
    g.nodes[g.startId].my = 0;
    grid[gridKey(0, 0)] = g.startId;

    std::set<int> placed = {g.startId};
    std::queue<int> mq;
    mq.push(g.startId);
    
    while (!mq.empty()) {
        int cur = mq.front(); 
        mq.pop();
        auto [mx, my] = std::make_pair(g.nodes[cur].mx, g.nodes[cur].my);
        std::vector<std::pair<int, int>> dirs = {{1,0},{0,1},{0,-1},{-1,0}};
        rng.sh(dirs);
        
        for (size_t ci = 0; ci < g.nodes[cur].children.size(); ++ci) {
            int cid = g.nodes[cur].children[ci];
            if (placed.count(cid)) continue;
            
            auto [dx, dy] = dirs[ci % dirs.size()];
            int tx = mx + dx, ty = my + dy;
            int attempts = 0;
            
            while (grid.count(gridKey(tx, ty)) && attempts < 20) {
                auto [fx, fy] = freeAround(mx, my);
                tx = fx; ty = fy; ++attempts;
            }
            
            g.nodes[cid].mx = tx; 
            g.nodes[cid].my = ty;
            grid[gridKey(tx, ty)] = cid;
            placed.insert(cid);
            mq.push(cid);
        }
    }

    // Seguro contra fallos: Ubicar cualquier nodo que se haya quedado atrás
    for (auto& n : g.nodes) {
        if (!placed.count(n.id)) {
            auto [fx, fy] = freeAround(0, 0);
            n.mx = fx; n.my = fy;
            grid[gridKey(fx, fy)] = n.id;
            placed.insert(n.id);
        }
    }
}

void MapGenerator::calculateStats(Graph& g) {
    int loopEdges = 0;
    for (const auto& e : g.edges) {
        if (e.isLoop) ++loopEdges;
    }
    
    g.stats = {
        static_cast<int>(g.nodes.size()),
        static_cast<int>(g.edges.size()),
        loopEdges,
        g.zones.earlyEnd,
        g.zones.midEnd,
        g.zones.maxDepth
    };
}