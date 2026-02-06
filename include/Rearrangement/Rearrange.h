#ifndef REARRANGE_H
#define REARRANGE_H

#include <SFML/Graphics.hpp>
#include "Rearrangement/RearrangeRender.h"
#include "Rearrangement/RearrangeEvents.h"
#include "Utils/Effect.h"
#include "Effects/ProjectileEffects/EffectsArrange.h"

class Rearrange {
public:
    static Rearrange* getInstance(EffectsArrange* effectsArrange = nullptr);
    ~Rearrange() = default;
    void openRearrangeMenu(sf::RenderWindow& window);
        
    void moveSelectionToNextEffect();
    void moveSelectionToPreviousEffect();
    void selectEffect();
    void closeRearrangeMenu();
    EffectsArrange* getEffectsArrange() const;

    std::string getLabelCode(EffectType type);
    sf::Color getColorCode(EffectType type);

private:
    Rearrange();
    
    int selectedEffectIndex;
    int selectedTargetIndex;
    bool isMovingEffect;

    bool isOpen;
    
    static EffectsArrange* _effectsArrange;
    static Rearrange* _instance;
    
    int getTotalSlots() const;
};
#endif // REARRANGE_H