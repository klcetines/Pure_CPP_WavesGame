#include "Map/MapRenderer.h"

#include <cmath>
#include <algorithm>

// ─────────────────────────────────────────────────────────────────────────────
//  Colour palette per room type
// ─────────────────────────────────────────────────────────────────────────────
Color MapRenderer::floorColor(RoomType type) const {
    switch (type) {
        case RoomType::START:   return Color(50,  80,  60);
        case RoomType::END:     return Color(80,  30,  70);
        case RoomType::BOSS:    return Color(80,  30,  20);
        case RoomType::UPGRADE: return Color(30,  60,  80);
        case RoomType::SHOP:    return Color(60,  50,  80);
        case RoomType::KEY:     return Color(80,  70,  20);
        default:                return Color(40,  40,  50);
    }
}

Color MapRenderer::wallColor(RoomType type) const {
    switch (type) {
        case RoomType::START:   return Color(90,  130, 100);
        case RoomType::END:     return Color(130, 60,  120);
        case RoomType::BOSS:    return Color(130, 50,  40);
        case RoomType::UPGRADE: return Color(60,  110, 140);
        case RoomType::SHOP:    return Color(110, 90,  140);
        case RoomType::KEY:     return Color(140, 120, 40);
        default:                return Color(70,  70,  85);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  build()
// ─────────────────────────────────────────────────────────────────────────────
void MapRenderer::build(const Graph& graph,
                        float                      playerSize,
                        const Vector2u&            windowSize)
{
    _playerSize  = playerSize*2;
    _windowSize  = windowSize;
    _startRoomId = graph.startId;

    // Camera view: same size as the window, no zoom.
    _cameraView.setSize(static_cast<float>(windowSize.x),
                        static_cast<float>(windowSize.y));

    buildRoomData(graph);

    // Start camera on the START room.
    _currentRoomId = graph.startId;
    setCameraCenter(currentRoomCenter());
}

// ─────────────────────────────────────────────────────────────────────────────
//  buildRoomData()  — iterate all nodes
// ─────────────────────────────────────────────────────────────────────────────
void MapRenderer::buildRoomData(const Graph& graph) {
    // First pass: fill _rooms with world origins and open doors.
    for (const auto& node : graph.nodes) {
        RoomRenderData room;
        room.nodeId      = node.id;
        room.type        = node.type;
        room.worldOrigin = worldOriginForNode(node);
        room.openDoors   = computeOpenDoors(node.id, graph);

        _rooms[node.id] = std::move(room);
    }

    // Build adjacency map: nodeId → [(neighbourId, directionToNeighbour)]
    for (const auto& edge : graph.edges) {
        const auto& nA = graph.nodes[edge.from];
        const auto& nB = graph.nodes[edge.to];

        auto dirAB = directionBetween(nA, nB);
        auto dirBA = directionBetween(nB, nA);

        if (dirAB) _adjacency[edge.from].emplace_back(edge.to,   *dirAB);
        if (dirBA) _adjacency[edge.to  ].emplace_back(edge.from, *dirBA);
    }

    // Second pass: build geometry for each room now that openDoors are known.
    for (auto& [id, room] : _rooms) {
        buildFloorVisuals(room);
        buildWallShapes(room);
        buildWallVisuals(room);
        buildGridVisuals(room);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  worldOriginForNode()
// ─────────────────────────────────────────────────────────────────────────────
Vector2f MapRenderer::worldOriginForNode(const Node& n) const {
    // mx, my are integer grid coordinates from MapGenerator::calculateMapLayout.
    // We scale them to pixel world space.
    return Vector2f(
        static_cast<float>(n.mx) * ROOM_SPACING_X,
        static_cast<float>(n.my) * ROOM_SPACING_Y
    );
}

// ─────────────────────────────────────────────────────────────────────────────
//  computeOpenDoors()
// ─────────────────────────────────────────────────────────────────────────────
std::vector<Direction> MapRenderer::computeOpenDoors(int nodeId,
                                                     const Graph& graph) const
{
    std::vector<Direction> doors;
    const auto& node = graph.nodes[nodeId];

    // For every edge involving this node, check map-grid adjacency.
    for (const auto& edge : graph.edges) {
        int otherId = -1;
        if (edge.from == nodeId) otherId = edge.to;
        else if (edge.to == nodeId) otherId = edge.from;
        if (otherId < 0) continue;

        const auto& other = graph.nodes[otherId];
        auto dir = directionBetween(node, other);
        if (dir) doors.push_back(*dir);
    }
    return doors;
}

// ─────────────────────────────────────────────────────────────────────────────
//  directionBetween()
// ─────────────────────────────────────────────────────────────────────────────
std::optional<Direction> MapRenderer::directionBetween(const Node& a,
                                                        const Node& b) const
{
    int dx = b.mx - a.mx;
    int dy = b.my - a.my;

    // Only consider cardinal-adjacent rooms (distance 1 in exactly one axis).
    if (std::abs(dx) + std::abs(dy) != 1) return std::nullopt;

    if (dx ==  1) return Direction::East;
    if (dx == -1) return Direction::West;
    if (dy ==  1) return Direction::South;
    if (dy == -1) return Direction::North;
    return std::nullopt;
}

// ─────────────────────────────────────────────────────────────────────────────
//  buildFloorVisuals()
// ─────────────────────────────────────────────────────────────────────────────
void MapRenderer::buildFloorVisuals(RoomRenderData& room) const {
    room.floorVA.setPrimitiveType(Quads);
    room.floorVA.clear();

    Color base  = floorColor(room.type);
    Color alt   = Color(base.r + 6, base.g + 6, base.b + 6);

    for (int row = 0; row < ROOM_ROWS; ++row) {
        for (int col = 0; col < ROOM_COLS; ++col) {
            float x = room.worldOrigin.x + col * CELL_SIZE;
            float y = room.worldOrigin.y + row * CELL_SIZE;
            Color c = ((row + col) % 2 == 0) ? base : alt;
            appendQuad(room.floorVA, x, y, CELL_SIZE, CELL_SIZE, c);
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  buildGridVisuals()  — debug overlay
// ─────────────────────────────────────────────────────────────────────────────
void MapRenderer::buildGridVisuals(RoomRenderData& room) const {
    room.gridVA.setPrimitiveType(Lines);
    room.gridVA.clear();

    Color lineCol(80, 80, 100, 60);
    float x0 = room.worldOrigin.x;
    float y0 = room.worldOrigin.y;
    float x1 = x0 + ROOM_PIXEL_W;
    float y1 = y0 + ROOM_PIXEL_H;

    // Vertical lines
    for (int col = 0; col <= ROOM_COLS; ++col) {
        float x = x0 + col * CELL_SIZE;
        room.gridVA.append(Vertex(Vector2f(x, y0), lineCol));
        room.gridVA.append(Vertex(Vector2f(x, y1), lineCol));
    }
    // Horizontal lines
    for (int row = 0; row <= ROOM_ROWS; ++row) {
        float y = y0 + row * CELL_SIZE;
        room.gridVA.append(Vertex(Vector2f(x0, y), lineCol));
        room.gridVA.append(Vertex(Vector2f(x1, y), lineCol));
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  buildWallShapes()  — collision geometry
//       ┌──────────────────────────────┐  ← North wall (y = origin.y - thickness)
//       │                              │
//       │   INTERIOR (ROOM_COLS×ROWS)  │
//       │                              │
//       └──────────────────────────────┘  ← South wall (y = origin.y + ROOM_H)
//       ↑West                        East↑
// ─────────────────────────────────────────────────────────────────────────────
void MapRenderer::buildWallShapes(RoomRenderData& room) const {
    room.wallShapes.clear();

    const float WALL_THICKNESS   = CELL_SIZE;
    const float OX  = room.worldOrigin.x;
    const float OY  = room.worldOrigin.y;
    const float W   = ROOM_PIXEL_W;
    const float H   = ROOM_PIXEL_H;

    // Gap half-width in pixels
    const float gapHalf = (CORRIDOR_WIDTH * CELL_SIZE) * 0.5f;

    // Helper lambda: is a given Direction in this room's openDoors?
    auto hasDir = [&](Direction d) {
        return std::find(room.openDoors.begin(), room.openDoors.end(), d)
               != room.openDoors.end();
    };

    // ── North wall ──────────────────
    {
        bool open      = hasDir(Direction::North);
        float gapCentre = OX + W * 0.5f;
        // wallAxis=true → horizontal wall
        addWallSegments(room.wallShapes,
                        /*wallAxis*/   true,
                        /*fixedCoord*/ OY - WALL_THICKNESS,
                        /*startCoord*/ OX - WALL_THICKNESS,
                        /*endCoord*/   OX + W + WALL_THICKNESS,
                        open, gapCentre, gapHalf, WALL_THICKNESS);
    }

    // ── South wall ────────────────────────────────────────────
    {
        bool open      = hasDir(Direction::South);
        float gapCentre = OX + W * 0.5f;
        addWallSegments(room.wallShapes,
                        true,
                        OY + H,
                        OX - WALL_THICKNESS,
                        OX + W + WALL_THICKNESS,
                        open, gapCentre, gapHalf, WALL_THICKNESS);
    }

    // ── West wall ──────────────────────
    {
        bool open      = hasDir(Direction::West);
        float gapCentre = OY + H * 0.5f;
        // wallAxis=false → vertical wall
        addWallSegments(room.wallShapes,
                        false,
                        OX - WALL_THICKNESS,
                        OY,
                        OY + H,
                        open, gapCentre, gapHalf, WALL_THICKNESS);
    }

    // ── East wall   (x = OX + W) ────────────────────────────────────────────
    {
        bool open      = hasDir(Direction::East);
        float gapCentre = OY + H * 0.5f;
        addWallSegments(room.wallShapes,
                        false,
                        OX + W,
                        OY,
                        OY + H,
                        open, gapCentre, gapHalf, WALL_THICKNESS);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  addWallSegments()
//
//  For a horizontal wall (wallAxis=true):
//      fixedCoord  = Y of the wall rectangle top-left
//      startCoord  = left X
//      endCoord    = right X
//      gapCenter   = X of corridor centre
//
//  For a vertical wall (wallAxis=false):
//      fixedCoord  = X of the wall rectangle left edge
//      startCoord  = top Y
//      endCoord    = bottom Y
//      gapCenter   = Y of corridor centre
// ─────────────────────────────────────────────────────────────────────────────
void MapRenderer::addWallSegments(std::vector<CollisionShape>& shapes,
                                   bool   wallAxis,
                                   float  fixedCoord,
                                   float  startCoord,
                                   float  endCoord,
                                   bool   hasGap,
                                   float  gapCenter,
                                   float  gapHalfW,
                                   float  wallThickness) const
{
    auto makeRect = [&](float freeStart, float freeEnd) {
        FloatRect rect;
        if (wallAxis) {
            // horizontal wall: free axis = X, fixed = Y
            rect = FloatRect(freeStart, fixedCoord,
                             freeEnd - freeStart, wallThickness);
        } 
        else {
            // vertical wall: free axis = Y, fixed = X
            rect = FloatRect(fixedCoord, freeStart,
                             wallThickness, freeEnd - freeStart);
        }
        shapes.emplace_back(rect);
    };

    if (!hasGap) {
        makeRect(startCoord, endCoord);
        return;
    }

    // Left / top segment
    float gapStart = gapCenter - gapHalfW;
    float gapEnd   = gapCenter + gapHalfW;

    if (gapStart > startCoord)
        makeRect(startCoord, gapStart);

    if (gapEnd < endCoord)
        makeRect(gapEnd, endCoord);
}

// ─────────────────────────────────────────────────────────────────────────────
//  buildWallVisuals()
// ─────────────────────────────────────────────────────────────────────────────
void MapRenderer::buildWallVisuals(RoomRenderData& room) const {
    room.wallVA.setPrimitiveType(Quads);
    room.wallVA.clear();

    Color wc = wallColor(room.type);

    // Visualise each CollisionShape's FloatRect directly.
    for (const auto& shape : room.wallShapes) {
        const FloatRect& r = shape.rect;
        appendQuad(room.wallVA, r.left, r.top, r.width, r.height, wc);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  appendQuad()
// ─────────────────────────────────────────────────────────────────────────────
void MapRenderer::appendQuad(VertexArray& va,
                               float x, float y, float w, float h,
                               Color color) const
{
    va.append(Vertex(Vector2f(x,     y    ), color));
    va.append(Vertex(Vector2f(x + w, y    ), color));
    va.append(Vertex(Vector2f(x + w, y + h), color));
    va.append(Vertex(Vector2f(x,     y + h), color));
}

// ─────────────────────────────────────────────────────────────────────────────
//  currentRoomCenter()
// ─────────────────────────────────────────────────────────────────────────────
Vector2f MapRenderer::currentRoomCenter() const {
    if (_rooms.count(_currentRoomId) == 0)
        return Vector2f(0.f, 0.f);

    const auto& room = _rooms.at(_currentRoomId);
    return Vector2f(
        room.worldOrigin.x + ROOM_PIXEL_W * 0.5f,
        room.worldOrigin.y + ROOM_PIXEL_H * 0.5f
    );
}

// ─────────────────────────────────────────────────────────────────────────────
//  startSpawnPosition()
// ─────────────────────────────────────────────────────────────────────────────
Vector2f MapRenderer::startSpawnPosition() const {
    if (_rooms.count(_startRoomId) == 0)
        return Vector2f(0.f, 0.f);

    const auto& room = _rooms.at(_startRoomId);
    return Vector2f(
        room.worldOrigin.x + ROOM_PIXEL_W * 0.5f,
        room.worldOrigin.y + ROOM_PIXEL_H * 0.5f
    );
}

// ─────────────────────────────────────────────────────────────────────────────
//  setCameraCenter()
// ─────────────────────────────────────────────────────────────────────────────
void MapRenderer::setCameraCenter(const Vector2f& center) {
    _cameraView.setCenter(center);
}

// ─────────────────────────────────────────────────────────────────────────────
//  roomAtPosition()
// ─────────────────────────────────────────────────────────────────────────────
int MapRenderer::roomAtPosition(const Vector2f& worldPos) const {
    for (const auto& [id, room] : _rooms) {
        FloatRect interior(room.worldOrigin.x, room.worldOrigin.y,
                           ROOM_PIXEL_W, ROOM_PIXEL_H);
        if (interior.contains(worldPos)) return id;
    }
    return -1;
}

void MapRenderer::discoverRoom(int roomId) {
    _discoveredRooms.insert(roomId);
}

void MapRenderer::drawMinimap(sf::RenderWindow& window, sf::Vector2f playerPosition) const {
    sf::View originalView = window.getView();

    sf::View minimapView;
    
    minimapView.setViewport(sf::FloatRect(0.02f, 0.75f, 0.2f, 0.2f)); 
    
    minimapView.setCenter(playerPosition);
    minimapView.zoom(4.0f); 
    
    window.setView(minimapView);

    sf::RectangleShape bg(minimapView.getSize());
    bg.setOrigin(bg.getSize().x / 2.f, bg.getSize().y / 2.f);
    bg.setPosition(playerPosition);
    bg.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(bg);

    sf::RenderStates states;
    
    for (int id : _discoveredRooms) {
        if (_rooms.count(id) == 0) continue;
        const auto& room = _rooms.at(id);

        window.draw(room.floorVA, states);
        window.draw(room.wallVA, states);
        
        if (id != _currentRoomId) {
            // Lógica opcional para ensombrecer zonas antiguas
        }
    }

    sf::CircleShape playerDot(20.f);
    playerDot.setFillColor(sf::Color::Green);
    playerDot.setOrigin(20.f, 20.f);
    playerDot.setPosition(playerPosition);
    window.draw(playerDot);

    window.setView(originalView);
}

// ─────────────────────────────────────────────────────────────────────────────
//  update()
// ─────────────────────────────────────────────────────────────────────────────
void MapRenderer::update(float dt, const Vector2f& playerWorldPos) {
    // ── Advance ongoing transition ─────────────────────────────────────────
    if (_transition.active) {
        _transition.progress += dt / _transition.duration;
        if (_transition.progress >= 1.0f) {
            _transition.progress = 1.0f;
            _transition.active   = false;
            _currentRoomId       = _transition.toRoom;
            setCameraCenter(_transition.cameraTo);
            discoverRoom(_currentRoomId);   
        } else {
            // Smooth step interpolation for the camera.
            float t = _transition.progress;
            t = t * t * (3.f - 2.f * t);   // smoothstep
            Vector2f camPos = _transition.cameraFrom +
                              ((_transition.cameraTo - _transition.cameraFrom) * t);
            setCameraCenter(camPos);
        }
        return;
    }

    // ── Detect room change ─────────────────────────────────────────────────
    int detectedRoom = roomAtPosition(playerWorldPos);
    if (detectedRoom < 0 || detectedRoom == _currentRoomId) return;

    // Player has stepped into a new room — find direction of travel.
    std::optional<Direction> dir;
    if (_adjacency.count(_currentRoomId)) {
        for (const auto& [nbId, nbDir] : _adjacency.at(_currentRoomId)) {
            if (nbId == detectedRoom) { dir = nbDir; break; }
        }
    }

    // Build target camera centre for the new room.
    Vector2f camTo(0.f, 0.f);
    if (_rooms.count(detectedRoom))
        camTo = Vector2f(_rooms.at(detectedRoom).worldOrigin.x + ROOM_PIXEL_W * 0.5f,
                         _rooms.at(detectedRoom).worldOrigin.y + ROOM_PIXEL_H * 0.5f);

    // Launch transition.
    _transition.active    = true;
    _transition.fromRoom  = _currentRoomId;
    _transition.toRoom    = detectedRoom;
    _transition.direction = dir.value_or(Direction::East);
    _transition.progress  = 0.f;
    _transition.cameraFrom = _cameraView.getCenter();
    _transition.cameraTo   = camTo;
}

// ─────────────────────────────────────────────────────────────────────────────
//  getActiveWallShapes()
// ─────────────────────────────────────────────────────────────────────────────
std::vector<const CollisionShape*> MapRenderer::getActiveWallShapes() const {
    std::vector<const CollisionShape*> result;

    auto addRoom = [&](int id) {
        if (_rooms.count(id) == 0) return;
        for (const auto& shape : _rooms.at(id).wallShapes)
            result.push_back(&shape);
    };

    addRoom(_currentRoomId);
    if (_transition.active)
        addRoom(_transition.toRoom);

    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
//  draw()
// ─────────────────────────────────────────────────────────────────────────────
void MapRenderer::draw(RenderWindow& window, sf::Vector2f cameraPos, bool debugGrid) const {

    sf::RenderStates states;
    states.transform.translate(cameraPos.x, cameraPos.y);

    std::vector<int> toDraw = { _currentRoomId };
    if (_transition.active && _transition.toRoom >= 0)
        toDraw.push_back(_transition.toRoom);

    for (int id : toDraw) {
        if (_rooms.count(id) == 0) continue;
        const auto& room = _rooms.at(id);

        window.draw(room.floorVA, states);
        window.draw(room.wallVA, states);
        if (debugGrid) window.draw(room.gridVA, states);
    }
}
