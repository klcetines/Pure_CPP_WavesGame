#include "Rearrangement/RearrangeRender.h"
#include "Rearrangement/Rearrange.h"

void RearrangeRender::renderRearrangeMenu(Rearrange& rearrange, sf::RenderWindow& window){
    window.clear(sf::Color(15, 15, 15));
    renderEffectsArrange(rearrange, window);
}

void RearrangeRender::renderEffectsArrange(Rearrange& rearrange, sf::RenderWindow& window){
    EffectsArrange* effects = rearrange.getEffectsArrange();
    if (!effects) return;

    const auto& modifiers = effects->getModifiers();
    const auto& impacts = effects->getImpacts();

    float startX = 100.0f;
    float startY = 125.0f;
    float boxSize = 60.0f;
    float padding = 15.0f;
    float spacing = boxSize + padding;
    
    int impactIdx = 0;
    int maxModifiers = effects->getMaxModifiers();

    for (int i = 0; i < maxModifiers; ++i) {
        float xPos = startX + spacing * i;

        if (i < modifiers.size() && modifiers[i] != nullptr) {
            const auto& modifier = modifiers[i];
            
            std::string label = getLabelCode(modifier->GetType());
            sf::Color col = getColorCode(modifier->GetType());

            drawEffectSlot(window, xPos, startY, boxSize, label, col);

            if (modifier->extraImpact()) { 
                float impactX = xPos;
                if (impactIdx < impacts.size()) {
                    drawEffectSlot(window, impactX, startY + spacing, boxSize, "I", sf::Color::Red);
                    impactIdx++;
                } else {
                    drawEffectSlot(window, impactX, startY + spacing, boxSize, "I", sf::Color(50, 50, 50, 100));
                }
            }
        } 
        else {
            drawEffectSlot(window, xPos, startY, boxSize, "", sf::Color(50, 50, 50, 50));
        }
    }

    sf::Color lastImpactCol = (impactIdx < impacts.size()) ? sf::Color::Red : sf::Color(50, 50, 50, 50);
    drawEffectSlot(window, startX + spacing * maxModifiers, startY + spacing, boxSize, "I", lastImpactCol);
}

void RearrangeRender::drawEffectSlot(sf::RenderWindow& window, float x, float y, float size, const std::string& label, const sf::Color& color) {
    sf::RectangleShape box(sf::Vector2f(size, size));
    box.setPosition(x, y);
    
    sf::Color fillColor = color;
    fillColor.a = 80; 
    box.setFillColor(fillColor);
    
    box.setOutlineColor(color);
    box.setOutlineThickness(2.0f);
    
    window.draw(box);

    if (!label.empty()) {
        static sf::Font font;
        static bool fontLoaded = font.loadFromFile("assets/fonts/Circle.otf"); 

        sf::Text t(label, font, 20);
        sf::FloatRect bounds = t.getLocalBounds();
        t.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
        t.setPosition(x + size / 2.0f, y + size / 2.0f);
        t.setFillColor(sf::Color::White);
        window.draw(t);
    }
}

std::string RearrangeRender::getLabelCode(EffectType type) {
    switch (type) {
        case EffectType::Piercing:
            return "P";
        case EffectType::Homing:
            return "H";
        default:
            return "M";
    }
}

sf::Color RearrangeRender::getColorCode(EffectType type) {
    switch (type) {
        case EffectType::Piercing:
            return sf::Color::Yellow;
        case EffectType::Homing:
            return sf::Color::Cyan;
        default:
            return sf::Color::Cyan;
    }
}