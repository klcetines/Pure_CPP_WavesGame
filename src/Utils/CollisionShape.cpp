#include "Utils/CollisionShape.h"
#include <cmath>

CollisionShape::CollisionShape() : type(ShapeType::Circle), center(0,0), radius(0), height(0), rotationDeg(0) {}
CollisionShape::CollisionShape(const sf::Vector2f& c, float r) : type(ShapeType::Circle), center(c), radius(r), height(0), rotationDeg(0) {}
CollisionShape::CollisionShape(const sf::FloatRect& r) : type(ShapeType::Rectangle), rect(r), center(0,0), radius(0), height(0), rotationDeg(0) {}
CollisionShape::CollisionShape(const sf::Vector2f& c, float w, float h, float rot) : type(ShapeType::Capsule), center(c), height(h), rotationDeg(rot), radius(0) {}

bool CollisionShape::intersects(const CollisionShape& other) const {
    if (type == ShapeType::Circle) {
        return circleIntersections(other);
    }
    else if (type == ShapeType::Rectangle) {
        return rect.intersects(other.rect);
    }
    else if (type == ShapeType::Capsule) {
        return capsuleIntersections(other);
    }

    return false;
}

bool CollisionShape::circleIntersections(const CollisionShape& other) const {
    if (type != ShapeType::Circle || other.type != ShapeType::Circle) return false;
    return circleCollides(center, radius, other.center, other.radius);
}

bool CollisionShape::circleCollides(const sf::Vector2f& centerA, float radiusA, const sf::Vector2f& centerB, float radiusB) const{
    float dx = centerA.x - centerB.x;
    float dy = centerA.y - centerB.y;

    float distSq = dx * dx + dy * dy;
    float radSum = radiusA + radiusB;

    return distSq <= (radSum * radSum);
}

bool CollisionShape::rectangleIntersects(const CollisionShape& other) const {
    if (type != ShapeType::Rectangle || other.type != ShapeType::Rectangle) return false;
    return rect.intersects(other.rect);
}

bool CollisionShape::capsuleIntersections(const CollisionShape& other) const {
    if (other.type != ShapeType::Circle) return capsuleCollides(center, radius, height, rotationDeg, other.center, other.radius);
    else if (other.type == ShapeType::Capsule) {
        return capsuleCollisionOnCapsule(other);

    }
    else if (other.type == ShapeType::Rectangle) {
        //TBD
    }
    return false;
}

bool CollisionShape::capsuleCollides(const sf::Vector2f& centerA, float radiusA, float heightA, float rotationDegA,
                                     const sf::Vector2f& centerB, float radiusB) const {
    //Pre: Recieves a Capsule and a Circle
    //Post: Returns true if the Capsule and Circle collide, false otherwise

    Vector2f closestAxisPoint = findClosestPointOnCapsuleAxis(centerA, radiusA, heightA, rotationDegA, centerB);
    
    if(circleCollides(closestAxisPoint, radiusA, centerB, radiusB)) return true;
    else return false;
}

Vector2f CollisionShape::findClosestPointOnCapsuleAxis(const Vector2f& centerA, float radius, float height, float rotationDeg, const Vector2f& point) const {
    // Pre: Recieves a Capsule and a Point
    // Post: Returns the closest point on the Capsule Axis to the Point

    Vector2f capsuleLastCenter = rotatePoint(centerA.x, centerA.y, height, rotationDeg);

    Vector2f axisDir = capsuleLastCenter - centerA;
    float axisLen = sqrt(axisDir.x * axisDir.x + axisDir.y * axisDir.y);

    axisDir /= axisLen;

    Vector2f toB = point - centerA;
    float t = toB.x * axisDir.x + toB.y * axisDir.y;

    t = max(0.0f, min(height, t));

    Vector2f closestPoint = centerA + axisDir * t;

    return closestPoint;
}

Vector2f CollisionShape::rotatePoint(float x_A, float y_A, float height, float angleDeg) const {
    float angleRad = angleDeg * 3.14159265f / 180.0f;

    float y_rel = y_A + height;
    
    float x_out = x_A - (y_rel * sin(angleRad));
    float y_out = y_A + (y_rel * cos(angleRad));

    return Vector2f(x_out, y_out);
}

bool CollisionShape::capsuleCollisionOnCapsule(const CollisionShape& other) const {
    // Pre: Recieves two Capsules
    // Post: Returns true if the Capsules collide, false otherwise

    if (type != ShapeType::Capsule || other.type != ShapeType::Capsule) return false;

    Vector2f thisLastCenter = rotatePoint(center.x, center.y, height, rotationDeg);
    Vector2f otherLastCenter = rotatePoint(other.center.x, other.center.y, other.height, other.rotationDeg);

    Vector2f otherClosestPoint = findOtherClosestPoint(center, thisLastCenter, other.center, otherLastCenter);

    return capsuleCollides(center, radius, height, rotationDeg, otherClosestPoint, other.radius);
}

Vector2f CollisionShape::findOtherClosestPoint(Vector2f firstA, Vector2f lastA, Vector2f firstB, Vector2f lastB) const {
    // Pre: Recieves 4 points representing the first and last points of two Capsules axis
    // Post: Returns the closest point on the B Capsule axis to the A Capsule axis

    Vector2f v0 = firstB - firstA;
    Vector2f v1 = lastB - firstA;
    Vector2f v2 = firstB - lastA;
    Vector2f v3 = lastB - lastA;

    float d0 = v0.x * v0.x + v0.y * v0.y;
    float d1 = v1.x * v1.x + v1.y * v1.y;
    float d2 = v2.x * v2.x + v2.y * v2.y;
    float d3 = v3.x * v3.x + v3.y * v3.y;

    Vector2f bestA;
    if (d2 < d0 || d2 < d1 || d3 < d0 || d3 < d1) {
        bestA = lastA; 
    } else {
        bestA = firstA;
    }

    return closestPointOnLineSegment(firstB, lastB, bestA);
}

Vector2f CollisionShape::closestPointOnLineSegment(Vector2f A, Vector2f B, Vector2f point) const {
    // Pre: Recieves two points A and B representing a line segment and a point
    // Post: Returns the closest point on the line segment to the point

    Vector2f AB = B - A;
    float t = ((point - A).x * AB.x + (point - A).y * AB.y) / (AB.x * AB.x + AB.y * AB.y);
    t = clamp(t, 0.0f, 1.0f);

    return A + AB * t;
}