#include "Utils/ActorUI.h"
#include "Enemies/Enemy.h"
#include <cmath>

ActorUI::ActorUI(Enemy* enemy) : _enemy(enemy) {}

void ActorUI::drawEffectUI(sf::RenderWindow& window, sf::Vector2f pos, float progress, sf::Color color, int charges) {
    static sf::Font font;
    if (!font.loadFromFile("assets/fonts/Circle.otf")) std::cerr << "Error loading font" << std::endl;

    sf::CircleShape bg(10.0f);
    bg.setOrigin(10.0f, 10.0f);
    bg.setPosition(pos);
    bg.setFillColor(sf::Color(50, 50, 50, 150));
    window.draw(bg);

    if (progress > 0.0f) {
        int pointCount = 30; 
        sf::VertexArray vertices(sf::TriangleFan);
        
        vertices.append(sf::Vertex(pos, sf::Color(color.r, color.g, color.b, 200)));

        float startAngle = -3.14159f / 2.0f; 
        float angleRange = (2.0f * 3.14159f) * progress;

        for (int i = 0; i <= pointCount; ++i) {
            float angle = startAngle + (angleRange * i / pointCount);
            sf::Vector2f edgePos(
                pos.x + std::cos(angle) * 10.0f, 
                pos.y + std::sin(angle) * 10.0f
            );
            vertices.append(sf::Vertex(edgePos, sf::Color(color.r, color.g, color.b, 200)));
        }
        window.draw(vertices);
    }
    
    sf::Text chargeText(std::to_string(charges), font, 12);
    chargeText.setPosition(pos.x - 5.0f, pos.y - 5.0f);
    chargeText.setFillColor(sf::Color::White);
    window.draw(chargeText);
}

void ActorUI::draw(sf::RenderWindow& window, float offsetX, float offsetY) {
    if (!_enemy) return;

    ActorEffectComponent* effectComp = _enemy->getEffectComponent();
    
    const auto& effects = effectComp->GetActiveEffects(); 

    if (effects.empty()) return;

    sf::Vector2f uiPos = _enemy->getHeadPosition(); 
    uiPos.y -= 25.0f; 
    uiPos.x += offsetX;
    uiPos.y += offsetY;

    float iconOffset = 25.0f; 
    uiPos.x -= (effects.size() - 1) * (iconOffset / 2.0f);

    for (const auto& pair : effects) {
        IActorEffect* effect = pair.second.get();
        
        float progress = effect->getTickProgress(); 
        
        sf::Color effectColor = sf::Color::White;
        if (pair.first == ActorEffectType::Fire) effectColor = sf::Color(255, 100, 0); 
        else if (pair.first == ActorEffectType::Bleed) effectColor = sf::Color(200, 0, 0); 

        drawEffectUI(window, uiPos, progress, effectColor, effect->getCharges());
        
        uiPos.x += iconOffset; 
    }
}