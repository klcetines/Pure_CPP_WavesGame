#pragma once
#include <SFML/Graphics.hpp>
#include "Utils/SpriteLoader.h"

class CharacterGraphics {
public:
    CharacterGraphics(float size);
    void loadSprite(const std::string& path, float x, float y);
    void draw(sf::RenderWindow& window, const sf::Vector2f& position, 
              float angle, bool isDamaged, float offsetX = 0, float offsetY = 0);
    void setDeadAppearance();

private:
    void initializeFallbackShape();
    
    sf::CircleShape shape;
    sf::Sprite sprite;
    sf::Texture texture;
    float size;
    bool useSprite;
};