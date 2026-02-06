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
    int maxModifiers = effects->getMaxModifiers();

    int totalImpactSlots = impacts.size();

    LayoutConfig layout;
    float totalWidth = layout.spacing * (maxModifiers + 1);
    layout.startX = (window.getSize().x - totalWidth) / 2.0f;
    layout.startY = (window.getSize().y - layout.spacing * 2) / 2.0f;

    RenderContext ctx = { window, layout, selectedIndex, targetIndex, isMovingEffect, maxModifiers};

    int impactIndex = 0;
    for (int i = 0; i < maxModifiers; ++i) {
        renderSingleModifier(ctx, i, modifiers);
        if(i < modifiers.size() && modifiers[i] && modifiers[i]->extraImpact()) {
            renderSingleImpact(ctx, i, impactIndex, maxModifiers, impacts);
            impactIndex++;
        }
    }
    renderSingleImpact(ctx, maxModifiers, impactIndex, maxModifiers, impacts);

    renderMovingOverlay(ctx, modifiers);
}

void RearrangeRender::renderSingleModifier(const RenderContext& ctx, int index, const std::vector<std::unique_ptr<IProjectileEffect>>& modifiers) {
    float xPos = ctx.layout.startX + ctx.layout.spacing * index;
    float yPos = ctx.layout.startY;

    std::string label = "";
    sf::Color color = sf::Color(50, 50, 50); 
    
    bool hasEffect = (index < modifiers.size() && modifiers[index] != nullptr);

    if (hasEffect) {
        label = getLabelCode(modifiers[index]->GetType());
        color = getColorCode(modifiers[index]->GetType());
    }

    bool isTarget = (index == (ctx.isMovingEffect ? ctx.targetIndex : ctx.selectedIndex));
    bool isSource = (ctx.isMovingEffect && index == ctx.selectedIndex);

    drawEffectSlot(ctx.window, xPos, yPos, ctx.layout.boxSize, label, color, isTarget, isSource);
}

void RearrangeRender::renderSingleImpact(const RenderContext& ctx, int index, int impactIndex, int modifiersSize, const std::vector<std::unique_ptr<IProjectileEffect>>& impacts) {
    float xPos = ctx.layout.startX + ctx.layout.spacing * index;
    float yPos = ctx.layout.startY + ctx.layout.spacing;

    std::string label = "";
    sf::Color color = sf::Color(50, 50, 50); 
    
    bool hasEffect = (impactIndex < impacts.size() && impacts[impactIndex] != nullptr);

    if (hasEffect) {
        label = getLabelCode(impacts[impactIndex]->GetType());
        color = getColorCode(impacts[impactIndex]->GetType());
    }
    int globalImpactIndex = modifiersSize + impactIndex;
    bool isTarget = (globalImpactIndex == (ctx.isMovingEffect ? ctx.targetIndex : ctx.selectedIndex));
    bool isSource = (ctx.isMovingEffect && globalImpactIndex == ctx.selectedIndex);

    drawEffectSlot(ctx.window, xPos, yPos, ctx.layout.boxSize, label, color, isTarget, isSource);
}

void RearrangeRender::renderMovingOverlay(const RenderContext& ctx, const std::vector<std::unique_ptr<IProjectileEffect>>& modifiers) {
    if (!ctx.isMovingEffect || ctx.selectedIndex < 0 || ctx.selectedIndex >= modifiers.size()) return;
    if (!modifiers[ctx.selectedIndex]) return;

    float xPos = ctx.layout.startX + ctx.layout.spacing * ctx.targetIndex;
    float yPos = ctx.layout.startY;

    std::string label = getLabelCode(modifiers[ctx.selectedIndex]->GetType());
    sf::Color color = getColorCode(modifiers[ctx.selectedIndex]->GetType());

    drawEffectSlot(ctx.window, xPos, yPos, ctx.layout.boxSize, label, color, true, false);
}

void RearrangeRender::drawBox(sf::RenderWindow& window, float x, float y, float size, sf::Color fillColor, sf::Color outlineColor, float outlineThickness) {
    sf::RectangleShape box(sf::Vector2f(size, size));
    box.setPosition(x, y);
    box.setFillColor(fillColor);
    box.setOutlineColor(outlineColor);
    box.setOutlineThickness(outlineThickness);
    window.draw(box);
}

void RearrangeRender::drawLabel(sf::RenderWindow& window, float x, float y, float size, const std::string& label, sf::Color color) {
    if (label.empty()) return;

    sf::Text t(label, getFont(), 20);
    sf::FloatRect bounds = t.getLocalBounds();
    t.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    t.setPosition(x + size / 2.0f, y + size / 2.0f);
    t.setFillColor(color);
    window.draw(t);
}

void RearrangeRender::drawEffectSlot(sf::RenderWindow& window, float x, float y, float size, const std::string& label, const sf::Color& color, bool isSelected, bool isSource) {    
    sf::Color fillColor = color;
    fillColor.a = isSource ? 30 : 80;
    sf::Color outlineColor = isSelected ? sf::Color::Green : color;
    float thickness = isSelected ? 3.0f : 2.0f;

    drawBox(window, x, y, size, fillColor, outlineColor, thickness);
    drawLabel(window, x, y, size, label, sf::Color::Black);
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

sf::Font& RearrangeRender::getFont() {
    static sf::Font font;
    static bool loaded = false;
    if (!loaded) {
        if (!font.loadFromFile("assets/fonts/Circle.otf")) {
            std::cerr << "Error loading font" << std::endl;
        }
        loaded = true;
    }
    return font;
}

