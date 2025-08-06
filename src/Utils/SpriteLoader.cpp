#include "Utils/SpriteLoader.h"

shared_ptr<Texture> SpriteLoader::getTexture(const string& path){
    static map<string, shared_ptr<Texture>> textures;
    auto it = textures.find(path);
    if (it != textures.end()) return it->second;

    auto tex = make_shared<Texture>();
    if (tex->loadFromFile(path)) {
        textures[path] = tex;
        return tex;
    }
    return nullptr;
}

bool SpriteLoader::assignSprite(Sprite& sprite, const string& path, float desiredWidth, float desiredHeight) {
    auto tex = getTexture(path);
    if (!tex) return false;
    sprite.setTexture(*tex);

    Vector2u texSize = tex->getSize();
    sprite.setOrigin(texSize.x / 2.0f, texSize.y / 2.0f);

    float scaleX = desiredWidth / texSize.x;
    float scaleY = desiredHeight / texSize.y;
    sprite.setScale(scaleX, scaleY);

    return true;
}