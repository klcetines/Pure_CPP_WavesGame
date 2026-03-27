#ifndef ACTOR_UI_H
#define ACTOR_UI_H

#include <SFML/Graphics.hpp>
#include "Effects/CharacterEffects/IActorEffect.h"
#include <map>
#include <memory>

class Enemy; 

class ActorUI {
public:
    ActorUI(Enemy* enemy);
    ~ActorUI() = default;

    void draw(sf::RenderWindow& window, float offsetX, float offsetY);

private:
    Enemy* _enemy;

    void drawEffectUI(sf::RenderWindow& window, sf::Vector2f pos, float progress, sf::Color color, int charges);
};

#endif // ACTOR_UI_H