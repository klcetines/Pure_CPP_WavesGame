#ifndef REARRANGE_H
#define REARRANGE_H

#include <SFML/Graphics.hpp>
#include "Rearrangement/RearrangeRender.h"
#include "Rearrangement/RearrangeEvents.h"
#include "Utils/Effect.h"
#include "Utils/EquipContext.h"

class Rearrange {
public:
    static Rearrange* getInstance(EquipContext ctx);

    ~Rearrange() = default;
    void openRearrangeMenu(sf::RenderWindow& window);
        
    void moveSelectionToNextEffect();
    void moveSelectionToPreviousEffect();
    void selectEffect();
    void closeRearrangeMenu();
    EquipContext* getEquipContext() const;

    std::string getLabelCode(EffectType type);
    sf::Color getColorCode(EffectType type);

    void swapSlotWithInventory(int slotIndex, int inventoryIndex);
    int  getTotalSlotsWithInventory() const;
    int  getTotalInventorySlots() const;
    int getTotalSlots() const;

private:
    Rearrange();
    
    int selectedEffectIndex;
    int selectedTargetIndex;
    bool isMovingEffect;

    bool isOpen;
    
    static EquipContext _ctx;
    static Rearrange* _instance;
    int nextValidIndex(int current, int delta) const;
    bool sourceEffectIsImpactType() const;
    
};
#endif // REARRANGE_H