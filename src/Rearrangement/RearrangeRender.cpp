#include "Rearrangement/RearrangeRender.h"
#include "Rearrangement/Rearrange.h"

static constexpr int INV_COLS = 5;

void RearrangeRender::renderRearrangeMenu(Rearrange& rearrange, sf::RenderWindow& window, int selectedIndex, int targetIndex, bool isMovingEffect){
    window.clear(sf::Color(15, 15, 15));
    renderEffectsArrange(rearrange, window, selectedIndex, targetIndex, isMovingEffect);
}

void RearrangeRender::renderEffectsArrange(Rearrange& rearrange, sf::RenderWindow& window, int selectedIndex, int targetIndex, bool isMovingEffect) {
    EquipContext* ctx = rearrange.getEquipContext();
    if (!ctx) return;

    const auto& modifiers = ctx->arrange->getModifiers();
    const auto& impacts   = ctx->arrange->getImpacts();
    const auto& inventory = ctx->inventory->getEffects();
    int maxModifiers      = ctx->arrange->getMaxModifiers();
    int totalSlots        = rearrange.getTotalSlots();
    int inventorySlots    = rearrange.getTotalInventorySlots();

    LayoutConfig layout;
    float arrangeWidth = layout.spacing * (maxModifiers + 1);
    layout.startX = (window.getSize().x - arrangeWidth) / 2.0f;
    layout.startY = (window.getSize().y - layout.spacing * 4) / 2.0f;

    RenderContext ctxRender = { window, layout, selectedIndex, targetIndex, isMovingEffect, maxModifiers };
    std::vector<int> impactVisualCol;

    int impactIndex = 0;
    for (int i = 0; i < maxModifiers; ++i) {
        renderSingleModifier(ctxRender, i, modifiers);
        if (i < (int)modifiers.size() && modifiers[i] && modifiers[i]->extraImpact()) {
            impactVisualCol.push_back(i);
            impactIndex++;
        }
    }

    impactVisualCol.push_back(maxModifiers);

    for (int imp = 0; imp < (int)impactVisualCol.size(); ++imp) {
        int col = impactVisualCol[imp];
        renderSingleImpact(ctxRender, col, imp, maxModifiers, impacts);
    }

    float invTotalWidth = layout.spacing * INV_COLS;
    float invStartX = (window.getSize().x - invTotalWidth) / 2.0f;
    float invStartY = layout.startY + layout.spacing * 2.f;

    renderInventoryRow(ctxRender, inventory, totalSlots, inventorySlots, invStartX, invStartY);
    renderMovingOverlay(ctxRender, modifiers, inventory, totalSlots, invStartX, invStartY);
}

void RearrangeRender::renderSingleModifier(const RenderContext& ctx, int index, const std::vector<std::unique_ptr<IProjectileEffect>>& modifiers) {
    float xPos = ctx.layout.startX + ctx.layout.spacing * index;
    float yPos = ctx.layout.startY;

    std::string label = "";
    sf::Color color = sf::Color(50, 50, 50);

    bool hasEffect = (index < (int)modifiers.size() && modifiers[index] != nullptr);
    if (hasEffect) {
        label = modifiers[index]->getModifierSymbol();
        uint32_t hex = modifiers[index]->getColorCode();
        color = sf::Color((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF);
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

    bool hasEffect = (impactIndex < (int)impacts.size() && impacts[impactIndex] != nullptr);
    if (hasEffect) {
        label = impacts[impactIndex]->getModifierSymbol();
        uint32_t hex = impacts[impactIndex]->getColorCode();
        color = sf::Color((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF);
    }

    int globalImpactIndex = modifiersSize + impactIndex;
    bool isTarget = (globalImpactIndex == (ctx.isMovingEffect ? ctx.targetIndex : ctx.selectedIndex));
    bool isSource = (ctx.isMovingEffect && globalImpactIndex == ctx.selectedIndex);

    drawEffectSlot(ctx.window, xPos, yPos, ctx.layout.boxSize, label, color, isTarget, isSource);
}

void RearrangeRender::renderInventoryRow(
    const RenderContext& ctx,
    const std::vector<std::unique_ptr<IProjectileEffect>>& inventory,
    int globalOffset,
    int inventorySlots,
    float startX,
    float startY)
{
    for (int i = 0; i < inventorySlots; ++i) {
        int col = i % INV_COLS;
        int row = i / INV_COLS;

        float xPos = startX + ctx.layout.spacing * col;
        float yPos = startY + ctx.layout.spacing * row;

        std::string label = "";
        sf::Color color = sf::Color(50, 50, 50);

        bool hasEffect = (i < (int)inventory.size() && inventory[i] != nullptr);
        if (hasEffect) {
            label = inventory[i]->getModifierSymbol();
            uint32_t hex = inventory[i]->getColorCode();
            color = sf::Color((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF);
        }

        int globalIndex = globalOffset + i;
        bool isTarget = (globalIndex == (ctx.isMovingEffect ? ctx.targetIndex : ctx.selectedIndex));
        bool isSource  = (ctx.isMovingEffect && globalIndex == ctx.selectedIndex);

        drawEffectSlot(ctx.window, xPos, yPos, ctx.layout.boxSize, label, color, isTarget, isSource);
    }
}

void RearrangeRender::renderMovingOverlay(
    const RenderContext& ctx,
    const std::vector<std::unique_ptr<IProjectileEffect>>& modifiers,
    const std::vector<std::unique_ptr<IProjectileEffect>>& inventory,
    int inventoryOffset,
    float invStartX,
    float invStartY)
{
    if (!ctx.isMovingEffect) return;

    const IProjectileEffect* srcEffect = nullptr;
    bool srcIsInventory = ctx.selectedIndex >= inventoryOffset;

    if (!srcIsInventory) {
        if (ctx.selectedIndex < 0 || ctx.selectedIndex >= (int)modifiers.size()) return;
        srcEffect = modifiers[ctx.selectedIndex].get();
    } else {
        int invIdx = ctx.selectedIndex - inventoryOffset;
        if (invIdx < 0 || invIdx >= (int)inventory.size()) return;
        srcEffect = inventory[invIdx].get();
    }

    if (!srcEffect) return;

    float xPos, yPos;
    bool dstIsInventory = ctx.targetIndex >= inventoryOffset;

    if (dstIsInventory) {
        int localIdx = ctx.targetIndex - inventoryOffset;
        int col = localIdx % INV_COLS;
        int row = localIdx / INV_COLS;
        xPos = invStartX + ctx.layout.spacing * col;
        yPos = invStartY + ctx.layout.spacing * row;
    } else {
        xPos = ctx.layout.startX + ctx.layout.spacing * ctx.targetIndex;
        yPos = ctx.layout.startY;
    }

    std::string label = srcEffect->getModifierSymbol();
    uint32_t hex = srcEffect->getColorCode();
    sf::Color color = sf::Color((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF);

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
    drawLabel(window, x, y, size, label, sf::Color::White);
}

std::string RearrangeRender::getLabelCode(EffectType type) {
    switch (type) {
        case EffectType::Piercing: return "P";
        case EffectType::Homing:   return "H";
        default:                   return "M";
    }
}

sf::Color RearrangeRender::getColorCode(EffectType type) {
    switch (type) {
        case EffectType::Piercing: return sf::Color::Yellow;
        case EffectType::Homing:   return sf::Color::Cyan;
        default:                   return sf::Color::Cyan;
    }
}

sf::Font& RearrangeRender::getFont() {
    static sf::Font font;
    static bool loaded = false;
    if (!loaded) {
        if (!font.loadFromFile("assets/fonts/Circle.otf"))
            std::cerr << "Error loading font" << std::endl;
        loaded = true;
    }
    return font;
}