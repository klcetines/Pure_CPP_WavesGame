#ifndef REARRANGE_RENDER_H
#define REARRANGE_RENDER_H

#include <SFML/Graphics.hpp>
#include "Effects/ProjectileEffects/IProjectileEffect.h"

class Rearrange;

namespace RearrangeRender {
    void renderRearrangeMenu(Rearrange& rearrange, sf::RenderWindow& window, int selectedIndex, int targetIndex, bool isMovingEffect);
    void renderEffectsArrange(Rearrange& rearrange, sf::RenderWindow& window, int selectedIndex, int targetIndex, bool isMovingEffect);
    void drawGhostEffect(sf::RenderWindow& window, float x, float y, float size, const std::string& label, const sf::Color& color);
    void drawEffectSlot(sf::RenderWindow& window, float x, float y, float size, const std::string& label, const sf::Color& color, bool isSelected, bool isSource);
    std::string getLabelCode(EffectType type);
    sf::Color getColorCode(EffectType type);
}

#endif // REARRANGE_RENDER_H