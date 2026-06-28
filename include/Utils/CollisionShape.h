#ifndef COLLISION_SHAPE_H
#define COLLISION_SHAPE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <optional>


enum class ShapeType { Circle, Rectangle, Capsule };

class CollisionShape {
public:
    ShapeType type;
    sf::Vector2f center;
    float radius;   
    sf::Vector2f  halfSize;   
    float height;
    float rotationDeg;

    // ── Constructors ────────────────────────────────────────────────────────
    CollisionShape();
    CollisionShape(const sf::Vector2f& center, float radius);
    CollisionShape(const sf::Vector2f& center, const sf::Vector2f& halfSize);
    CollisionShape(const sf::Vector2f& center, float radius, float height, float rotationDeg);

    // ── Detection ───────────────────────────────────────────────────────────
    bool intersects(const CollisionShape& other) const;
   
    // ── Resolution ──────────────────────────────────────────────────────────
    std::optional<sf::Vector2f> resolve(const CollisionShape& other) const;


    // ── Capsule helpers ─────────────────────────────────────────────────────
    sf::Vector2f getFirstCenter() const;
    sf::Vector2f getLastCenter()  const;

private:
    // ── Intersection helpers ─────────────────────────────────────────────
    bool circleIntersections(const CollisionShape& other) const;
    bool rectangleIntersections(const CollisionShape& other) const;
    bool capsuleIntersections(const CollisionShape& other) const;
    
    // ── Resolution helpers ───────────────────────────────────────────────
    std::optional<sf::Vector2f> resolveCircleCircle (const CollisionShape& other) const;
    std::optional<sf::Vector2f> resolveCircleRect (const CollisionShape& other) const;
    std::optional<sf::Vector2f> resolveRectRect (const CollisionShape& other) const;
    std::optional<sf::Vector2f> resolveCapsuleCircle (const CollisionShape& other) const;
    std::optional<sf::Vector2f> resolveCapsuleCapsule (const CollisionShape& other) const;
    std::optional<sf::Vector2f> resolveCapsuleRect (const CollisionShape& other) const;

    // ── Math utilities ────────────────────────────────────────────────────
    float pointToSegmentDistSq(const sf::Vector2f& p,
                                const sf::Vector2f& a,
                                const sf::Vector2f& b) const;
    sf::Vector2f closestPointOnSegment(const sf::Vector2f& p,
                                   const sf::Vector2f& a,
                                   const sf::Vector2f& b) const;
    float crossProduct2D(const sf::Vector2f& v, const sf::Vector2f& w) const;
    bool  segmentsIntersect(const sf::Vector2f& p1, const sf::Vector2f& p2,
                             const sf::Vector2f& p3, const sf::Vector2f& p4) const;
    sf::Vector2f closestPointOnRect(const sf::Vector2f& p,
                                             const sf::Vector2f& rCenter,
                                             const sf::Vector2f& rHalf) const;
                             
};

#endif // COLLISION_SHAPE_H