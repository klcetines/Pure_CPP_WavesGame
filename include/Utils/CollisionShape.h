#ifndef COLLISION_SHAPE_H
#define COLLISION_SHAPE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace sf;
using namespace std;

enum class ShapeType { Circle, Rectangle, Capsule };

class CollisionShape {
public:
    ShapeType type;
    Vector2f center;
    float radius;      
    FloatRect rect;
    float height;
    float rotationDeg;

    // Constructores
    CollisionShape();
    CollisionShape(const Vector2f& center, float radius); 
    CollisionShape(const FloatRect& rect);
    CollisionShape(const Vector2f& center, float radius, float height, float rotationDeg);

    // Función principal pública
    bool intersects(const CollisionShape& other) const;
    
    Vector2f getFirstCenter() const;
    Vector2f getLastCenter() const;

private:
    // Resolutores específicos por forma
    bool circleIntersections(const CollisionShape& other) const;
    bool rectangleIntersects(const CollisionShape& other) const;
    bool capsuleIntersections(const CollisionShape& other) const;
    
    // Herramientas matemáticas para Cápsulas
    float pointToSegmentDistSq(const Vector2f& p, const Vector2f& a, const Vector2f& b) const;
    float crossProduct2D(const Vector2f& v, const Vector2f& w) const;
    bool segmentsIntersect(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, const Vector2f& p4) const;
    
};

#endif // COLLISION_SHAPE_H