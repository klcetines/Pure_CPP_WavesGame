#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;

enum class ShapeType { Circle, Rectangle, Capsule };

class CollisionShape {
public:
    ShapeType type;
    Vector2f center;
    float radius;      
    FloatRect rect;
    float height, rotationDeg;

    CollisionShape();
    CollisionShape(const Vector2f& center, float radius); 
    CollisionShape(const FloatRect& rect);
    CollisionShape(const Vector2f& center, float radius, float height, float rotationDeg);

    // Static collision checks
    static bool circleContains(const Vector2f& center, float radius, const Vector2f& point);
    static bool rectContains(const FloatRect& rect, const Vector2f& point);
    static bool capsuleContains(const Vector2f& center, float width, float height, float rotationDeg, const Vector2f& point);

    // Shape vs Shape
    bool intersects(const CollisionShape& other) const;

    void drawDebug() const;

    Vector2f rotatePoint(float x_A, float y_A, float height, float angleDeg) const;    


private:
    bool circleCollides(const Vector2f& centerA, float radiusA, const Vector2f& centerB, float radiusB) const;
    bool circleIntersections(const CollisionShape& other) const;
    bool rectangleIntersects(const CollisionShape& other) const;
    bool capsuleIntersections(const CollisionShape& other) const;
    bool capsuleCollides(const Vector2f& centerA, float radiusA, float heightA, float rotationDegA, const Vector2f& centerB, float radiusB) const;
    Vector2f findClosestPointOnCapsuleAxis(const Vector2f& center, float width, float height, float rotationDeg, const Vector2f& point) const;
    bool capsuleCollisionOnCapsule(const CollisionShape& other) const;
    Vector2f findOtherClosestPoint(Vector2f firstA, Vector2f lastA, Vector2f firstB, Vector2f lastB) const;
    Vector2f closestPointOnLineSegment(Vector2f A, Vector2f B, Vector2f point) const;
};