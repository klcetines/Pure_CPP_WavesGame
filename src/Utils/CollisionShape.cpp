#include "Utils/CollisionShape.h"

// ─────────────────────────────────────────────────────────────────────────────
//  Constructors
// ─────────────────────────────────────────────────────────────────────────────
CollisionShape::CollisionShape()
    : type(ShapeType::Circle), center(0.f, 0.f),
      radius(0.f), height(0.f), rotationDeg(0.f) {}

CollisionShape::CollisionShape(const Vector2f& center, float radius)
    : type(ShapeType::Circle), center(center),
      radius(radius), height(0.f), rotationDeg(0.f) {}

CollisionShape::CollisionShape(const Vector2f& center, const Vector2f& halfSize)
    : type(ShapeType::Rectangle), center(center),
      radius(0.f), halfSize(halfSize), height(0.f), rotationDeg(0.f) {}

CollisionShape::CollisionShape(const Vector2f& center, float radius,
                                float height, float rotationDeg)
    : type(ShapeType::Capsule), center(center),
      radius(radius), height(height), rotationDeg(rotationDeg) {}

// ─────────────────────────────────────────────────────────────────────────────
//  intersects()  — public dispatcher
// ─────────────────────────────────────────────────────────────────────────────
bool CollisionShape::intersects(const CollisionShape& other) const {
    switch (type) {
        case ShapeType::Circle:    return circleIntersections(other);
        case ShapeType::Rectangle: return rectangleIntersections(other);
        case ShapeType::Capsule:   return capsuleIntersections(other);
    }
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
//  resolve()  — public dispatcher
//  Pre: Recieves another CollisionShape `other` that overlaps with *this*.
//  Post: Returns the vector that moves *this* out of `other`, or nullopt if no overlap.
// ─────────────────────────────────────────────────────────────────────────────
std::optional<Vector2f> CollisionShape::resolve(const CollisionShape& other) const {
    if (type == ShapeType::Circle) {
        if (other.type == ShapeType::Circle)    return resolveCircleCircle(other);
        if (other.type == ShapeType::Rectangle) return resolveCircleRect(other);
        if (other.type == ShapeType::Capsule) {
            auto mtv = other.resolveCapsuleCircle(*this);
            if (mtv) return Vector2f(-mtv->x, -mtv->y);
            return std::nullopt;
        }
    }
    if (type == ShapeType::Rectangle) {
        if (other.type == ShapeType::Rectangle) return resolveRectRect(other);
        if (other.type == ShapeType::Circle) {
            auto mtv = other.resolveCircleRect(*this);
            if (mtv) return Vector2f(-mtv->x, -mtv->y);
            return std::nullopt;
        }
        if (other.type == ShapeType::Capsule) {
            auto mtv = other.resolveCapsuleRect(*this);
            if (mtv) return Vector2f(-mtv->x, -mtv->y);
            return std::nullopt;
        }
    }
    if (type == ShapeType::Capsule) {
        if (other.type == ShapeType::Circle)    return resolveCapsuleCircle(other);
        if (other.type == ShapeType::Capsule)   return resolveCapsuleCapsule(other);
        if (other.type == ShapeType::Rectangle) return resolveCapsuleRect(other);
    }
    return std::nullopt;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Capsule axis endpoints
// ─────────────────────────────────────────────────────────────────────────────
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

// ─────────────────────────────────────────────────────────────────────────────
//  Intersection: Circle
// ─────────────────────────────────────────────────────────────────────────────
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
        Vector2f closest = closestPointOnRect(center, other.center, other.halfSize);
        float dx = center.x - closest.x;
        float dy = center.y - closest.y;
        return dx * dx + dy * dy <= radius * radius;
    }
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Intersection: Capsule
// ─────────────────────────────────────────────────────────────────────────────
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
        Vector2f a1 = getFirstCenter();
        Vector2f a2 = getLastCenter();

        Vector2f axisPoint = closestPointOnSegment(other.center, a1, a2);

        CollisionShape tempCircle(axisPoint, radius);

        return tempCircle.circleIntersections(other);
    }
    return false;
}

bool CollisionShape::rectangleIntersections(const CollisionShape& other) const {
    if (other.type == ShapeType::Circle)
        return other.circleIntersections(*this);
 
    if (other.type == ShapeType::Rectangle) {
        float dx = std::abs(center.x - other.center.x);
        float dy = std::abs(center.y - other.center.y);
        return dx < (halfSize.x + other.halfSize.x) &&
               dy < (halfSize.y + other.halfSize.y);
    }
 
    if (other.type == ShapeType::Capsule) { 
        Vector2f capA = other.getFirstCenter();
        Vector2f capB = other.getLastCenter();

        Vector2f axisPoint = closestPointOnSegment(center, capA, capB);
        Vector2f closestOnRect = closestPointOnRect(axisPoint, center, halfSize);

        float dx = axisPoint.x - closestOnRect.x;
        float dy = axisPoint.y - closestOnRect.y;
        return dx * dx + dy * dy <= other.radius * other.radius;
    }
 
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Resolution: Circle vs Circle
// ─────────────────────────────────────────────────────────────────────────────
std::optional<Vector2f> CollisionShape::resolveCircleCircle(const CollisionShape& other) const {
    float dx   = center.x - other.center.x;
    float dy   = center.y - other.center.y;
    float distSq = dx * dx + dy * dy;
    float rSum = radius + other.radius;
    if (distSq >= rSum * rSum) return std::nullopt;

    float dist = std::sqrt(distSq);
    float penetration = rSum - dist;

    if (dist < 1e-6f) {
        return Vector2f(0.f, -penetration);
    }
    float nx = dx / dist;
    float ny = dy / dist;
    return Vector2f(nx * penetration, ny * penetration);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Resolution: Circle vs Rectangle
// ─────────────────────────────────────────────────────────────────────────────
std::optional<Vector2f> CollisionShape::resolveCircleRect(const CollisionShape& other) const {
    float dx = center.x - other.center.x;
    float dy = center.y - other.center.y;
 
    bool insideX = std::abs(dx) < other.halfSize.x;
    bool insideY = std::abs(dy) < other.halfSize.y;
    bool inside  = insideX && insideY;
 
    if (inside) {
        float overlapLeft   = other.halfSize.x + dx + radius;
        float overlapRight  = other.halfSize.x - dx + radius;
        float overlapTop    = other.halfSize.y + dy + radius;
        float overlapBottom = other.halfSize.y - dy + radius;
 
        float minOv = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});
        if      (minOv == overlapLeft)   return Vector2f(-overlapLeft,  0.f);
        else if (minOv == overlapRight)  return Vector2f( overlapRight, 0.f);
        else if (minOv == overlapTop)    return Vector2f(0.f, -overlapTop);
        else                             return Vector2f(0.f,  overlapBottom);
    }
 
    Vector2f closest = closestPointOnRect(center, other.center, other.halfSize);
    float cx = center.x - closest.x;
    float cy = center.y - closest.y;
    float distSq = cx * cx + cy * cy;
    if (distSq >= radius * radius) return std::nullopt;
 
    float dist = std::sqrt(distSq);
    float pen  = radius - dist;
    if (dist < 1e-6f) return Vector2f(0.f, -pen);
    return Vector2f((cx / dist) * pen, (cy / dist) * pen);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Resolution: Rectangle vs Rectangle  (SAT — AABB)
// ─────────────────────────────────────────────────────────────────────────────
std::optional<Vector2f> CollisionShape::resolveRectRect(const CollisionShape& other) const {
    float dx = center.x - other.center.x;
    float dy = center.y - other.center.y;
 
    float overlapX = (halfSize.x + other.halfSize.x) - std::abs(dx);
    float overlapY = (halfSize.y + other.halfSize.y) - std::abs(dy);
 
    if (overlapX <= 0.f || overlapY <= 0.f) return std::nullopt;
 
    if (overlapX < overlapY)
        return Vector2f((dx > 0 ? overlapX : -overlapX), 0.f);
    else
        return Vector2f(0.f, (dy > 0 ? overlapY : -overlapY));
}

// ─────────────────────────────────────────────────────────────────────────────
//  Resolution: Capsule vs Circle
// ─────────────────────────────────────────────────────────────────────────────
std::optional<Vector2f> CollisionShape::resolveCapsuleCircle(const CollisionShape& other) const {
    Vector2f a1 = getFirstCenter();
    Vector2f a2 = getLastCenter();
    Vector2f closest = closestPointOnSegment(other.center, a1, a2);

    float dx = other.center.x - closest.x;
    float dy = other.center.y - closest.y;
    float distSq = dx * dx + dy * dy;
    float rSum = radius + other.radius;

    if (distSq >= rSum * rSum) return std::nullopt;

    float dist = std::sqrt(distSq);
    float penetration = rSum - dist;

    if (dist < 1e-6f) return Vector2f(0.f, -penetration);

    // MTV pushes the capsule away from the circle
    float nx = -dx / dist;
    float ny = -dy / dist;
    return Vector2f(nx * penetration, ny * penetration);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Resolution: Capsule vs Capsule  (approximate — segment-to-segment)
// ─────────────────────────────────────────────────────────────────────────────
std::optional<Vector2f> CollisionShape::resolveCapsuleCapsule(const CollisionShape& other) const {
    Vector2f a1 = getFirstCenter();
    Vector2f a2 = getLastCenter();
    Vector2f b1 = other.getFirstCenter();
    Vector2f b2 = other.getLastCenter();

    // Find pair of closest points between the two segments
    // Use the minimum over the four endpoint-to-segment distances
    struct Candidate { Vector2f onA, onB; float distSq; };
    auto check = [&](const Vector2f& p, const Vector2f& sa, const Vector2f& sb,
                     bool pOnA) -> Candidate {
        Vector2f closest = closestPointOnSegment(p, sa, sb);
        float dx = p.x - closest.x, dy = p.y - closest.y;
        return pOnA ? Candidate{p, closest, dx*dx+dy*dy}
                    : Candidate{closest, p, dx*dx+dy*dy};
    };

    Candidate best = check(a1, b1, b2, true);
    for (auto& c : {check(a2, b1, b2, true),
                    check(b1, a1, a2, false),
                    check(b2, a1, a2, false)}) {
        if (c.distSq < best.distSq) best = c;
    }

    float rSum = radius + other.radius;
    if (best.distSq >= rSum * rSum) return std::nullopt;

    float dist        = std::sqrt(best.distSq);
    float penetration = rSum - dist;
    if (dist < 1e-6f) return Vector2f(0.f, -penetration);

    float dx = best.onA.x - best.onB.x;
    float dy = best.onA.y - best.onB.y;
    return Vector2f((dx / dist) * penetration, (dy / dist) * penetration);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Resolution: Capsule vs Rectangle
//  Treat the capsule as a circle at its closest-axis-point to the rect.
// ─────────────────────────────────────────────────────────────────────────────
std::optional<Vector2f> CollisionShape::resolveCapsuleRect(const CollisionShape& other) const {
    Vector2f a1 = getFirstCenter();
    Vector2f a2 = getLastCenter();
 
    Vector2f axisPoint = closestPointOnSegment(other.center, a1, a2);
 
    CollisionShape tempCircle(axisPoint, radius);
    return tempCircle.resolveCircleRect(other);
}


// ─────────────────────────────────────────────────────────────────────────────
//  Math utilities
// ─────────────────────────────────────────────────────────────────────────────
Vector2f CollisionShape::closestPointOnSegment(const Vector2f& p,
                                                const Vector2f& a,
                                                const Vector2f& b) const
{
    Vector2f ab = b - a;
    float abLenSq = ab.x * ab.x + ab.y * ab.y;
    if (abLenSq == 0.f) return a;
    float t = ((p.x - a.x) * ab.x + (p.y - a.y) * ab.y) / abLenSq;
    t = std::max(0.f, std::min(1.f, t));
    return Vector2f(a.x + ab.x * t, a.y + ab.y * t);
}

float CollisionShape::pointToSegmentDistSq(const Vector2f& p,
                                            const Vector2f& a,
                                            const Vector2f& b) const
{
    Vector2f closest = closestPointOnSegment(p, a, b);
    Vector2f d = p - closest;
    return d.x * d.x + d.y * d.y;
}

Vector2f CollisionShape::closestPointOnRect(const Vector2f& p,
                                             const Vector2f& rCenter,
                                             const Vector2f& rHalf) const
{
    return Vector2f(
        std::max(rCenter.x - rHalf.x, std::min(p.x, rCenter.x + rHalf.x)),
        std::max(rCenter.y - rHalf.y, std::min(p.y, rCenter.y + rHalf.y))
    );
}

float CollisionShape::crossProduct2D(const Vector2f& v, const Vector2f& w) const {
    return v.x * w.y - v.y * w.x;
}

bool CollisionShape::segmentsIntersect(const Vector2f& p1, const Vector2f& p2,
                                        const Vector2f& p3, const Vector2f& p4) const
{
    Vector2f r       = p2 - p1;
    Vector2f s       = p4 - p3;
    Vector2f p3_p1   = p3 - p1;
    float denom      = crossProduct2D(r, s);
    if (denom == 0.f) return false;
    float t = crossProduct2D(p3_p1, s) / denom;
    float u = crossProduct2D(p3_p1, r) / denom;
    return (t >= 0.f && t <= 1.f && u >= 0.f && u <= 1.f);
}
