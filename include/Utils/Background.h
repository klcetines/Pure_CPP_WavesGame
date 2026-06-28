#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>



class Background {
    public:
        Background(const std::string& texturePath);
        ~Background() = default;
        void drawTiled(sf::RenderWindow& window, float px, float py, int screenW, int screenH);

    private:
        sf::Texture texture;
        sf::Sprite sprite;
};

#endif // BACKGROUND_H