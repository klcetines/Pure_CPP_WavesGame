#include "Character/CharacterGraphics.h"

CharacterGraphics::CharacterGraphics(float size)
    : size(size)
    , useSprite(false)
{
    initializeFallbackShape();
}

void CharacterGraphics::loadSprite(const string& path, float x, float y) {
    if (texture.loadFromFile(path)) {
        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
        float scale = (size * 2) / texture.getSize().x;
        sprite.setScale(scale, scale);
        sprite.setPosition(x, y);
        useSprite = true;
    }
}

void CharacterGraphics::draw(RenderWindow& window, const Vector2f& position, 
                           float angle, bool isDamaged, float offsetX, float offsetY) 
{
    static const int BLINK_FRAMES = 8;
    
    if (isDamaged) {
        static int blinkCounter = 0;
        blinkCounter++;
        if ((blinkCounter / BLINK_FRAMES) % 2 == 0) return;
    }

    Vector2f screenPos(position.x + offsetX, position.y + offsetY);

    if (useSprite) {
        sprite.setPosition(screenPos);
        sprite.setRotation(angle);
        sprite.setColor(isDamaged ? Color(255, 0, 0, 128) : Color::White);
        window.draw(sprite);
    } else {
        shape.setPosition(screenPos);
        shape.setRotation(angle);
        shape.setFillColor(isDamaged ? Color::Red : Color::Green);
        window.draw(shape);
    }
}

void CharacterGraphics::initializeFallbackShape() {
    shape.setRadius(size);
    shape.setPointCount(30);
    shape.setFillColor(Color::Green);
    shape.setOrigin(size, size);
}

void CharacterGraphics::setDeadAppearance() {
    shape.setFillColor(Color::Red);
}