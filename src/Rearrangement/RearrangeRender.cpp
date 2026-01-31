#include "Rearrangement/RearrangeRender.h"
#include "Rearrangement/Rearrange.h"

void RearrangeRender::renderRearrangeMenu(Rearrange& rearrange, sf::RenderWindow& window, int selectedIndex, int targetIndex, bool isMovingEffect){
    window.clear(sf::Color(15, 15, 15));
    renderEffectsArrange(rearrange, window, selectedIndex, targetIndex, isMovingEffect);
}

void RearrangeRender::renderEffectsArrange(Rearrange& rearrange, sf::RenderWindow& window, int selectedIndex, int targetIndex, bool isMovingEffect) {
    EffectsArrange* effects = rearrange.getEffectsArrange();
    if (!effects) return;

    const auto& modifiers = effects->getModifiers();
    const auto& impacts = effects->getImpacts();

    std::string movingLabel = "";
    sf::Color movingColor = sf::Color::White;
    if (isMovingEffect && selectedIndex >= 0) {
        if (selectedIndex < modifiers.size()) {
             if(modifiers[selectedIndex]) {
                movingLabel = getLabelCode(modifiers[selectedIndex]->GetType());
                movingColor = getColorCode(modifiers[selectedIndex]->GetType());
             }
        } 
    }

    float startX = 100.0f;
    float startY = 125.0f;
    float boxSize = 60.0f;
    float padding = 15.0f;
    float spacing = boxSize + padding;
    
    int currentGlobalIndex = 0;
    int impactIdx = 0;
    int maxModifiers = effects->getMaxModifiers();

    for (int i = 0; i < maxModifiers; ++i) {
        float xPos = startX + spacing * i;

        if (i < modifiers.size() && modifiers[i] != nullptr) {
            const auto& modifier = modifiers[i];
            
            std::string label = getLabelCode(modifier->GetType());
            sf::Color col = getColorCode(modifier->GetType());
            
            bool isSelectedPos = (currentGlobalIndex == (isMovingEffect ? targetIndex : selectedIndex));
            bool isSourcePos = (isMovingEffect && currentGlobalIndex == selectedIndex);
            
            sf::Color drawCol = col;
            if(isSourcePos) drawCol.a = 50;
            
            drawEffectSlot(window, xPos, startY, boxSize, label, drawCol, isSelectedPos, isSourcePos);

            if (isSelectedPos && isMovingEffect && !isSourcePos){
                drawEffectSlot(window, xPos, startY, boxSize, movingLabel, movingColor, true, false);
            }
            if (modifier->extraImpact()) { 
                float impactX = xPos;
                int currentImpactGlobalIndex = modifiers.size() + impactIdx; 
                bool isImpSelected = (currentImpactGlobalIndex == (isMovingEffect ? targetIndex : selectedIndex));
                
                if (impactIdx < impacts.size()) {
                    drawEffectSlot(window, impactX, startY + spacing, boxSize, "I", sf::Color::Red, isImpSelected, false);
                    impactIdx++;
                } else {
                    drawEffectSlot(window, impactX, startY + spacing, boxSize, "I", sf::Color(50, 50, 50, 100), isImpSelected, false);
                }
            }
        } 
        else {
            bool isSelectedPos = (currentGlobalIndex == (isMovingEffect ? targetIndex : selectedIndex));
            drawEffectSlot(window, xPos, startY, boxSize, "", sf::Color(50, 50, 50, 50), isSelectedPos, false);
        }
    }

    sf::Color lastImpactCol = (impactIdx < impacts.size()) ? sf::Color::Red : sf::Color(50, 50, 50, 50);
    drawEffectSlot(window, startX + spacing * maxModifiers, startY + spacing, boxSize, "I", lastImpactCol, (selectedIndex == (modifiers.size() + impactIdx)), false);
}

void RearrangeRender::drawGhostEffect(sf::RenderWindow& window, float x, float y, float size, const std::string& label, const sf::Color& color) {
    sf::RectangleShape box(sf::Vector2f(size, size));
    box.setPosition(x, y);
    
    sf::Color ghostColor = color;
    ghostColor.a = 120;
    
    box.setFillColor(ghostColor);
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(1.0f);
    
    window.draw(box);

    if (!label.empty()) {
        static sf::Font font;
        if(font.getInfo().family.empty()) font.loadFromFile("assets/fonts/Circle.otf");

        sf::Text t(label, font, 20);
        sf::FloatRect bounds = t.getLocalBounds();
        t.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
        t.setPosition(x + size / 2.0f, y + size / 2.0f);
        t.setFillColor(sf::Color(255, 255, 255, 200));
        window.draw(t);
    }
}

void RearrangeRender::drawEffectSlot(sf::RenderWindow& window, float x, float y, float size, const std::string& label, const sf::Color& color, bool isSelected, bool isSource) {    
    sf::RectangleShape box(sf::Vector2f(size, size));
    box.setPosition(x, y);
    
    sf::Color fillColor = color;
    if (isSource) {
        fillColor.a = 30;
    } 
    else {
        fillColor.a = 80;
    } 

    if (isSelected) {
        box.setOutlineColor(sf::Color::White);
        box.setOutlineThickness(3.0f);
    } else {
        box.setOutlineColor(color);
        box.setOutlineThickness(2.0f);
    }
    
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