#ifndef REARRANGE_RENDER_H
#define REARRANGE_RENDER_H

#include <SFML/Graphics.hpp>
#include "Effects/ProjectileEffects/IProjectileEffect.h"

class Rearrange;

namespace RearrangeRender {
    struct LayoutConfig {
        float boxSize = 120.0f;
        float padding = 15.0f;
        float spacing = 135.0f;
        float startX = 0.0f;
        float startY = 0.0f;
    };

    struct RenderContext {
        sf::RenderWindow& window;
        const LayoutConfig& layout;
        int selectedIndex;
        int targetIndex;
        bool isMovingEffect;
        int modifiersCount;
    };

    void renderRearrangeMenu(Rearrange& rearrange, sf::RenderWindow& window, int selectedIndex, int targetIndex, bool isMovingEffect);
    
    void renderEffectsArrange(Rearrange& rearrange, sf::RenderWindow& window, int selectedIndex, int targetIndex, bool isMovingEffect);
    
    void renderSingleModifier(const RenderContext& ctx, int index, const std::vector<std::unique_ptr<IProjectileEffect>>& modifiers);
    void renderSingleImpact(const RenderContext& ctx, int index, int impactIndex, int modifiersSize, const std::vector<std::unique_ptr<IProjectileEffect>>& impacts);
    void renderMovingOverlay(const RenderContext& ctx, const std::vector<std::unique_ptr<IProjectileEffect>>& modifiers);

    void drawBox(sf::RenderWindow& window, float x, float y, float size, sf::Color fillColor, sf::Color outlineColor, float outlineThickness);
    void drawLabel(sf::RenderWindow& window, float x, float y, float size, const std::string& label, sf::Color color);
    void drawEffectSlot(sf::RenderWindow& window, float x, float y, float size, const std::string& label, const sf::Color& color, bool isSelected, bool isSource);
    
    std::string getLabelCode(EffectType type);
    sf::Color getColorCode(EffectType type);
    sf::Font& getFont();
}

#endif // REARRANGE_RENDER_H