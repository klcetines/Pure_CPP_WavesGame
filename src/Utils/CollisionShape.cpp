#include "Utils/CollisionShape.h"

CollisionShape::CollisionShape() 
    : type(ShapeType::Circle), center(0.f, 0.f), radius(0.f), height(0.f), rotationDeg(0.f) {}

CollisionShape::CollisionShape(const Vector2f& center, float radius)
    : type(ShapeType::Circle), center(center), radius(radius), height(0.f), rotationDeg(0.f) {}

CollisionShape::CollisionShape(const FloatRect& rect)
    : type(ShapeType::Rectangle), rect(rect), center(rect.left + rect.width / 2.f, rect.top + rect.height / 2.f), radius(0.f), height(0.f), rotationDeg(0.f) {}

CollisionShape::CollisionShape(const Vector2f& center, float radius, float height, float rotationDeg)
    : type(ShapeType::Capsule), center(center), radius(radius), height(height), rotationDeg(rotationDeg) {}

bool CollisionShape::intersects(const CollisionShape& other) const {
    if (type == ShapeType::Circle) {
        return circleIntersections(other);
    }
    else if (type == ShapeType::Rectangle) {
        return rectangleIntersects(other);
    }
    else if (type == ShapeType::Capsule) {
        return capsuleIntersections(other);
    }
    return false;
}

bool CollisionShape::circleIntersections(const CollisionShape& other) const {
    if (other.type == ShapeType::Circle) {
        float dx = center.x - other.center.x;
        float dy = center.y - other.center.y;
        float distSq = dx * dx + dy * dy;
        float radiusSum = radius + other.radius;
        return distSq <= (radiusSum * radiusSum);
    }
    else if (other.type == ShapeType::Capsule) {
        Vector2f capA = other.getFirstCenter();
        Vector2f capB = other.getLastCenter();
        float distSq = pointToSegmentDistSq(center, capA, capB);
        float radiusSum = radius + other.radius;
        return distSq <= (radiusSum * radiusSum);
    }
    else if (other.type == ShapeType::Rectangle) {
        return false; 
    }
    return false;
}

bool CollisionShape::capsuleIntersections(const CollisionShape& other) const {
    if (other.type == ShapeType::Circle) {
        return other.circleIntersections(*this);
    } 
    else if (other.type == ShapeType::Capsule) {
        Vector2f a1 = getFirstCenter();
        Vector2f a2 = getLastCenter();
        Vector2f b1 = other.getFirstCenter();
        Vector2f b2 = other.getLastCenter();

        if (segmentsIntersect(a1, a2, b1, b2)) return true;

        float minDistSq = pointToSegmentDistSq(a1, b1, b2);
        minDistSq = std::min(minDistSq, pointToSegmentDistSq(a2, b1, b2));
        minDistSq = std::min(minDistSq, pointToSegmentDistSq(b1, a1, a2));
        minDistSq = std::min(minDistSq, pointToSegmentDistSq(b2, a1, a2));

        float radiusSum = radius + other.radius;
        return minDistSq <= (radiusSum * radiusSum);
    }
    else if (other.type == ShapeType::Rectangle) {
        return false;
    }
    return false;
}

bool CollisionShape::rectangleIntersects(const CollisionShape& other) const {
    if (other.type == ShapeType::Rectangle) {
        return rect.intersects(other.rect);
    }
    return false;
}

float CollisionShape::pointToSegmentDistSq(const Vector2f& p, const Vector2f& a, const Vector2f& b) const {
    Vector2f ab = b - a;
    Vector2f ap = p - a;
    
    float proj = ap.x * ab.x + ap.y * ab.y;
    float abLenSq = ab.x * ab.x + ab.y * ab.y;
    
    if (abLenSq == 0.0f) {
        return ap.x * ap.x + ap.y * ap.y;
    }
    
    float t = std::max(0.0f, std::min(1.0f, proj / abLenSq));
    Vector2f closestPoint = a + ab * t;
    Vector2f diff = p - closestPoint;
    
    return diff.x * diff.x + diff.y * diff.y;
}

float CollisionShape::crossProduct2D(const Vector2f& v, const Vector2f& w) const {
    return v.x * w.y - v.y * w.x;
}

bool CollisionShape::segmentsIntersect(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, const Vector2f& p4) const {
    Vector2f r = p2 - p1;
    Vector2f s = p4 - p3;
    Vector2f p3_p1 = p3 - p1;
    
    float uNumerator = crossProduct2D(p3_p1, r);
    float denominator = crossProduct2D(r, s);
    
    if (denominator == 0.0f) { return false; } 
    
    float u = uNumerator / denominator;
    float t = crossProduct2D(p3_p1, s) / denominator;
    
    return (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f);
}

Vector2f CollisionShape::getFirstCenter() const {
    float rad = rotationDeg * (M_PI / 180.0f);
    float halfHeight = height / 2.0f;
    return Vector2f(
        center.x + std::cos(rad) * halfHeight,
        center.y + std::sin(rad) * halfHeight
    );
}

Vector2f CollisionShape::getLastCenter() const {
    float rad = rotationDeg * (M_PI / 180.0f);
    float halfHeight = height / 2.0f;
    return Vector2f(
        center.x - std::cos(rad) * halfHeight,
        center.y - std::sin(rad) * halfHeight
    );
}