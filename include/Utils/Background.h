#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>


using namespace sf;
using namespace std;

class Background {
    public:
        Background(const string& texturePath);
        ~Background() = default;
        void drawTiled(RenderWindow& window, float px, float py, int screenW, int screenH);

    private:
        Texture texture;
        Sprite sprite;
};

#endif // BACKGROUND_H