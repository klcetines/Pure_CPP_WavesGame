#include "Background.h"

Background::Background(const string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        cerr << "No se pudo cargar la textura desde: " << texturePath << endl;
        exit(1);
    }
    sprite.setTexture(texture);
}

void Background::drawTiled(RenderWindow& window, float px, float py, int screenW, int screenH) {
    int tileW = texture.getSize().x;
    int tileH = texture.getSize().y;
    int tilesX = screenW / tileW + 2;
    int tilesY = screenH / tileH + 2;
    float offsetX = fmod(px, (float)tileW);
    float offsetY = fmod(py, (float)tileH);

    for (int i = -1; i < tilesX; ++i) {
        for (int j = -1; j < tilesY; ++j) {
            sprite.setPosition(i * tileW - offsetX, j * tileH - offsetY);
            window.draw(sprite);
        }
    }
}