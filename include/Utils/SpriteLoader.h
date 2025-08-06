#ifndef SPRITELOADER_H
#define SPRITELOADER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <memory>

using namespace std;
using namespace sf;

class SpriteLoader {
public:

    static shared_ptr<Texture> getTexture(const string& path);
    static bool assignSprite(Sprite& sprite, const string& path, float desiredWidth, float desiredHeight);

};

#endif