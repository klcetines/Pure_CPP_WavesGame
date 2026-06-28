#ifndef SPRITELOADER_H
#define SPRITELOADER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <memory>


class SpriteLoader {
public:

    static std::shared_ptr<sf::Texture> getTexture(const std::string& path);
    static bool assignSprite(sf::Sprite& sprite, const std::string& path, float desiredWidth, float desiredHeight);

};

#endif