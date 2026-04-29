#ifndef MAP_RENDERER_H
#define MAP_RENDERER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <optional>

#include "Utils/CollisionShape.h"
#include "Map/MapGenerator.h"

using namespace sf;


// ─────────────────────────────────────────────────────────────────────────────
//  CONSTANTS
// ─────────────────────────────────────────────────────────────────────────────
static constexpr float CELL_SIZE        = 40.0f;

static constexpr int   ROOM_COLS        = 15;   // X cells
static constexpr int   ROOM_ROWS        = 11;   // Y cells

static constexpr int   CORRIDOR_WIDTH   = 3;    // cells

static constexpr float ROOM_PIXEL_W     = ROOM_COLS * CELL_SIZE;
static constexpr float ROOM_PIXEL_H     = ROOM_ROWS * CELL_SIZE;

static constexpr float ROOM_SPACING_X   = ROOM_PIXEL_W  + CELL_SIZE * 4.0f;
static constexpr float ROOM_SPACING_Y   = ROOM_PIXEL_H  + CELL_SIZE * 4.0f;

enum class Direction { North, South, East, West };

inline Direction opposite(Direction d) {
    switch (d) {
        case Direction::North: return Direction::South;
        case Direction::South: return Direction::North;
        case Direction::East:  return Direction::West;
        case Direction::West:  return Direction::East;
    }
    return Direction::North;
}


struct RoomRenderData {
    int                         nodeId      = -1;
    RoomType                    type        = RoomType::NORMAL;
    Vector2f                    worldOrigin;        // top-left of interior (pixels)

    std::vector<Direction>      openDoors;

    std::vector<CollisionShape> wallShapes;

    VertexArray                 floorVA;
    VertexArray                 wallVA;
    VertexArray                 gridVA;
};

struct RoomTransition {
    bool        active      = false;
    int         fromRoom    = -1;
    int         toRoom      = -1;
    Direction   direction   = Direction::East;
    float       progress    = 0.0f;   // 0 → 1
    float       duration    = 0.35f;  // seconds
    Vector2f    cameraFrom;
    Vector2f    cameraTo;
};

class MapRenderer {
public:

    MapRenderer() = default;

    void build(const Graph& graph,
               float                      playerSize,
               const Vector2u&            windowSize);

    void update(float dt, const Vector2f& playerWorldPos);

    void draw(RenderWindow& window, sf::Vector2f cameraPos, bool debugGrid = false) const;

    std::vector<const CollisionShape*> getActiveWallShapes() const;
    Vector2f currentRoomCenter() const;
    int currentRoomId() const { return _currentRoomId; }
    int roomAtPosition(const Vector2f& worldPos) const;

    Vector2f startSpawnPosition() const;

    void discoverRoom(int roomId);
    
    void drawMinimap(sf::RenderWindow& window, sf::Vector2f playerPosition) const;

private:
    void buildRoomData(const Graph& graph);
    void buildSingleRoom(RoomRenderData&           room,
                         const Node& node,
                         const Graph& graph);

    Vector2f worldOriginForNode(const Node& n) const;

    std::vector<Direction> computeOpenDoors(int nodeId,
                                            const Graph& graph) const;

    std::optional<Direction> directionBetween(const Node& a,
                                              const Node& b) const;

    void buildWallShapes(RoomRenderData& room) const;

    void buildWallVisuals(RoomRenderData& room) const;

    void buildFloorVisuals(RoomRenderData& room) const;

    void buildGridVisuals(RoomRenderData& room) const;

    void appendQuad(VertexArray& va,
                    float x, float y, float w, float h,
                    Color color) const;

    // ── Wall collision segment factory ─────────────────────────────────────

    void addWallSegments(std::vector<CollisionShape>& shapes,
                         bool   wallAxis,
                         float  fixedCoord,
                         float  startCoord,
                         float  endCoord,
                         bool   hasGap,
                         float  gapCenter,
                         float  gapHalfW,
                         float  wallThickness) const;

    // ── Camera ──────────────────────────────────────────────────────────────

    void updateCamera(float dt);
    void setCameraCenter(const Vector2f& center);

    // ── Room colours ────────────────────────────────────────────────────────
    Color floorColor(RoomType type) const;
    Color wallColor(RoomType type)  const;

    // ── Data ────────────────────────────────────────────────────────────────

    std::unordered_map<int, RoomRenderData> _rooms;

    std::unordered_map<int, std::vector<std::pair<int, Direction>>> _adjacency;

    int          _currentRoomId = -1;
    int          _startRoomId   = -1;

    View         _cameraView;
    Vector2u     _windowSize;
    float        _playerSize     = CELL_SIZE;

    std::unordered_set<int> _discoveredRooms;
    RoomTransition _transition;
};

#endif // MAP_RENDERER_H
