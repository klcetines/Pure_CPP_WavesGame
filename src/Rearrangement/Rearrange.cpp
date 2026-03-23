#include "Rearrangement/Rearrange.h"

EquipContext Rearrange::_ctx;
Rearrange* Rearrange::_instance = nullptr;

Rearrange* Rearrange::getInstance(EquipContext ctx) {
    if (_instance == nullptr) _instance = new Rearrange();
    _ctx = ctx;
    return _instance;
}


void Rearrange::openRearrangeMenu(sf::RenderWindow& window){
    isOpen = true;
    isMovingEffect = false; 
    selectedEffectIndex = 0;
    selectedTargetIndex = 0;
    while (isOpen && window.isOpen()){
        sf::Event event;
            RearrangeEvents::handleRearrangeInput(*this, window, isOpen);
            RearrangeRender::renderRearrangeMenu(*this, window, selectedEffectIndex, selectedTargetIndex, isMovingEffect);
            window.display();
            sf::sleep(sf::milliseconds(10));
        
    }
}

Rearrange::Rearrange() {
    isOpen = false;
    _ctx.arrange = nullptr;
    selectedEffectIndex = -1;
    selectedTargetIndex = -1;
    isMovingEffect = false;
}

int Rearrange::nextValidIndex(int current, int delta) const {
    int maxMods    = _ctx.arrange->getMaxModifiers();
    int totalSlots = getTotalSlots();
    int total      = getTotalSlotsWithInventory();

    int srcIndex     = isMovingEffect ? selectedEffectIndex : current;
    bool srcIsMod    = srcIndex < maxMods;
    bool srcIsImpact = (srcIndex >= maxMods && srcIndex < totalSlots);
    bool srcIsInv    = srcIndex >= totalSlots;

    bool movingImpactType = isMovingEffect && sourceEffectIsImpactType();

    int next = (current + delta + total) % total;

    for (int i = 0; i < total; ++i) {
        bool nextIsMod    = next < maxMods;
        bool nextIsImpact = (next >= maxMods && next < totalSlots);
        bool nextIsInv    = next >= totalSlots;

        bool allowed = false;

        if (!isMovingEffect) {
            allowed = true; 
        } 
        else if (movingImpactType) {
            allowed = nextIsImpact || nextIsInv;
        } 
        else {
            allowed = nextIsMod || nextIsInv;
        }

        if (allowed) return next;
        next = (next + delta + total) % total;
    }
    return current;
}

void Rearrange::moveSelectionToNextEffect() {
    int total = getTotalSlotsWithInventory();
    if (total <= 0) return;
    if (isMovingEffect)
        selectedTargetIndex = nextValidIndex(selectedTargetIndex, +1);
    else
        selectedEffectIndex = nextValidIndex(selectedEffectIndex, +1);
}

void Rearrange::moveSelectionToPreviousEffect() {
    int total = getTotalSlotsWithInventory();
    if (total <= 0) return;
    if (isMovingEffect)
        selectedTargetIndex = nextValidIndex(selectedTargetIndex, -1);
    else
        selectedEffectIndex = nextValidIndex(selectedEffectIndex, -1);
}

void Rearrange::selectEffect() {
    if (!_ctx.arrange) return;

    int totalSlots   = getTotalSlots();
    int maxModifiers = _ctx.arrange->getMaxModifiers();
    int maxImpacts   = _ctx.arrange->getMaxImpacts();

    if (!isMovingEffect) {
        bool srcIsInventory = selectedEffectIndex >= totalSlots;

        if (srcIsInventory) {
            if (!_ctx.inventory) return;
            int invIdx = selectedEffectIndex - totalSlots;
            auto& inv = _ctx.inventory->getEffects();
            if (invIdx >= (int)inv.size() || !inv[invIdx]) return;

        } else if (selectedEffectIndex < maxModifiers) {
            if (selectedEffectIndex >= (int)_ctx.arrange->getModifiers().size()) return;

        } else {
            int rel = selectedEffectIndex - maxModifiers;
            if (rel >= maxImpacts) return;
            if (rel >= (int)_ctx.arrange->getImpacts().size()) return;
        }

        isMovingEffect = true;
        selectedTargetIndex = selectedEffectIndex;

    } 
    else {
        if (selectedEffectIndex != selectedTargetIndex) {
            bool srcIsInventory = selectedEffectIndex >= totalSlots;
            bool dstIsInventory = selectedTargetIndex >= totalSlots;
            bool srcIsImpact    = !srcIsInventory && selectedEffectIndex >= maxModifiers;
            bool dstIsImpact    = !dstIsInventory && selectedTargetIndex >= maxModifiers;
            bool srcIsMod       = !srcIsInventory && selectedEffectIndex < maxModifiers;
            bool dstIsMod       = !dstIsInventory && selectedTargetIndex < maxModifiers;

            if (srcIsInventory || dstIsInventory) {
                bool movingImpact = sourceEffectIsImpactType();
                if (srcIsInventory) {
                    int invIdx = selectedEffectIndex - totalSlots;
                    if (dstIsImpact || (!movingImpact && dstIsMod)) {
                        swapSlotWithInventory(selectedTargetIndex, invIdx);
                    }
                } else {
                    int invIdx = selectedTargetIndex - totalSlots;
                    swapSlotWithInventory(selectedEffectIndex, invIdx);
                }

            } 
            else if (srcIsImpact && dstIsImpact) {
                // Impact ↔ Impact
                _ctx.arrange->swapEffects(selectedEffectIndex, selectedTargetIndex);

            } 
            else if (srcIsMod && dstIsMod) {
                // Modifier ↔ Modifier
                _ctx.arrange->swapEffects(selectedEffectIndex, selectedTargetIndex);

            }
        }
        isMovingEffect = false;
    }
}
void Rearrange::closeRearrangeMenu() {
    isOpen = false;
}

EquipContext* Rearrange::getEquipContext() const {
    return &_ctx;
}

int Rearrange::getTotalSlots() const {
    if (!_ctx.arrange) return 1;
    
    int modSlots = _ctx.arrange->getMaxModifiers();
    
    int impactSlots = _ctx.arrange->getMaxImpacts(); 
    
    return modSlots + impactSlots;
}

int Rearrange::getTotalInventorySlots() const {
    if (!_ctx.inventory) return 0;
    return _ctx.inventory->getMaxInventorySize();
}

int Rearrange::getTotalSlotsWithInventory() const {
    return getTotalSlots() + getTotalInventorySlots();
}

void Rearrange::swapSlotWithInventory(int slotIndex, int inventoryIndex) {
    if (!_ctx.isValid()) return;

    int maxMods = _ctx.arrange->getMaxModifiers();
    auto& inv   = _ctx.inventory->getEffects();

    bool invHasEffect = inventoryIndex < (int)inv.size() && inv[inventoryIndex] != nullptr;

    if (slotIndex >= maxMods) {
        // --- Impact slot ↔ inventory ---
        int impIdx = slotIndex - maxMods;
        auto& imps = _ctx.arrange->getImpacts();

        if (!invHasEffect) return;

        bool slotHasEffect = impIdx < (int)imps.size() && imps[impIdx] != nullptr;
        if (slotHasEffect) {
            std::swap(imps[impIdx], inv[inventoryIndex]);
        } else {
            while ((int)imps.size() <= impIdx) imps.push_back(nullptr);
            imps[impIdx] = std::move(inv[inventoryIndex]);
            inv.erase(inv.begin() + inventoryIndex);
        }
    } else {
        // --- Modifier slot ↔ inventory ---
        auto& mods = _ctx.arrange->getModifiers();
        bool slotHasEffect = slotIndex < (int)mods.size() && mods[slotIndex] != nullptr;

        if (!invHasEffect) return;

        if (slotHasEffect) {
            bool outHadExtra = mods[slotIndex]->extraImpact();
            bool inHasExtra  = inv[inventoryIndex]->extraImpact();
            if (outHadExtra && !inHasExtra) {
                int extraImpactIdx = 0;
                auto& imps = _ctx.arrange->getImpacts();
                for (int i = 0; i < slotIndex && i < (int)mods.size(); ++i) {
                    if (mods[i] && mods[i]->extraImpact()) extraImpactIdx++;
                }
                if (extraImpactIdx < (int)imps.size() && imps[extraImpactIdx]) {
                    if (!_ctx.inventory->isFull()) {
                        inv.push_back(std::move(imps[extraImpactIdx]));
                    }
                    imps.erase(imps.begin() + extraImpactIdx);
                }
            }

            std::swap(mods[slotIndex], inv[inventoryIndex]);

            if (outHadExtra != inHasExtra) _ctx.arrange->recalcMaxImpacts();

        } 
        else {
            // Empty Slot
            mods[slotIndex] = std::move(inv[inventoryIndex]);
            inv.erase(inv.begin() + inventoryIndex);
            if (mods[slotIndex] && mods[slotIndex]->extraImpact()) {
                _ctx.arrange->recalcMaxImpacts();
            }
        }
    }
}

bool Rearrange::sourceEffectIsImpactType() const {
    if (!_ctx.isValid()) return false;
    int totalSlots = getTotalSlots();
    int maxMods    = _ctx.arrange->getMaxModifiers();

    if (selectedEffectIndex >= totalSlots) {
        int invIdx = selectedEffectIndex - totalSlots;
        auto& inv = _ctx.inventory->getEffects();
        if (invIdx < (int)inv.size() && inv[invIdx])
            return inv[invIdx]->GetType() == EffectType::Impact;
        return false;
    }
    if (selectedEffectIndex >= maxMods) {
        return true;
    }
    return false;
}