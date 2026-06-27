#ifndef COLLISION_SHAPE_H
#define COLLISION_SHAPE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <optional>

using namespace sf;
using namespace std;

enum class ShapeType { Circle, Rectangle, Capsule };

class CollisionShape {
public:
    ShapeType type;
    Vector2f center;
    float radius;   
    Vector2f  halfSize;   
    float height;
    float rotationDeg;

    // ── Constructors ────────────────────────────────────────────────────────
    CollisionShape();
    CollisionShape(const Vector2f& center, float radius);
    CollisionShape(const Vector2f& center, const Vector2f& halfSize);
    CollisionShape(const Vector2f& center, float radius, float height, float rotationDeg);

    // ── Detection ───────────────────────────────────────────────────────────
    bool intersects(const CollisionShape& other) const;
   
    // ── Resolution ──────────────────────────────────────────────────────────
    std::optional<Vector2f> resolve(const CollisionShape& other) const;


    // ── Capsule helpers ─────────────────────────────────────────────────────
    Vector2f getFirstCenter() const;
    Vector2f getLastCenter()  const;

private:
    // ── Intersection helpers ─────────────────────────────────────────────
    bool circleIntersections(const CollisionShape& other) const;
    bool rectangleIntersections(const CollisionShape& other) const;
    bool capsuleIntersections(const CollisionShape& other) const;
    
    // ── Resolution helpers ───────────────────────────────────────────────
    std::optional<Vector2f> resolveCircleCircle (const CollisionShape& other) const;
    std::optional<Vector2f> resolveCircleRect (const CollisionShape& other) const;
    std::optional<Vector2f> resolveRectRect (const CollisionShape& other) const;
    std::optional<Vector2f> resolveCapsuleCircle (const CollisionShape& other) const;
    std::optional<Vector2f> resolveCapsuleCapsule (const CollisionShape& other) const;
    std::optional<Vector2f> resolveCapsuleRect (const CollisionShape& other) const;

    // ── Math utilities ────────────────────────────────────────────────────
    float pointToSegmentDistSq(const Vector2f& p,
                                const Vector2f& a,
                                const Vector2f& b) const;
    Vector2f closestPointOnSegment(const Vector2f& p,
                                   const Vector2f& a,
                                   const Vector2f& b) const;
    float crossProduct2D(const Vector2f& v, const Vector2f& w) const;
    bool  segmentsIntersect(const Vector2f& p1, const Vector2f& p2,
                             const Vector2f& p3, const Vector2f& p4) const;
    Vector2f closestPointOnRect(const Vector2f& p,
                                             const Vector2f& rCenter,
                                             const Vector2f& rHalf) const;
                             
};

#endif // COLLISION_SHAPE_H