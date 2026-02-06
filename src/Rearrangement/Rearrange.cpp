#include "Rearrangement/Rearrange.h"

Rearrange* Rearrange::_instance = nullptr;
EffectsArrange* Rearrange::_effectsArrange = nullptr;

Rearrange* Rearrange::getInstance(EffectsArrange* effectsArrange) {
    if (_instance == nullptr) {
        _instance = new Rearrange();
    }
    _effectsArrange = effectsArrange;
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
    _effectsArrange = nullptr;
    selectedEffectIndex = -1;
    selectedTargetIndex = -1;
    isMovingEffect = false;
}

void Rearrange::moveSelectionToNextEffect() {
    int totalSlots = getTotalSlots();
    if (totalSlots <= 0) return;

    if (isMovingEffect) {
        selectedTargetIndex = (selectedTargetIndex + 1) % totalSlots;
    } 
    else {
        selectedEffectIndex = (selectedEffectIndex + 1) % totalSlots;
    }
    cout << "Selected Effect Index: " << selectedEffectIndex << ", Target Index: " << selectedTargetIndex << std::endl;

}

void Rearrange::moveSelectionToPreviousEffect() {
    int totalSlots = getTotalSlots();
    if (totalSlots <= 0) return;

    if (isMovingEffect) {
        selectedTargetIndex = (selectedTargetIndex - 1 + totalSlots) % totalSlots;
    } 
    else {
        selectedEffectIndex = (selectedEffectIndex - 1 + totalSlots) % totalSlots;
    }
}

void Rearrange::selectEffect() {
    if (!_effectsArrange) return;

    int maxModifiers = _effectsArrange->getMaxModifiers();

    if (!isMovingEffect) {
        bool modifierEmpty = selectedEffectIndex < maxModifiers;
        
        if (modifierEmpty) {
            if (selectedEffectIndex >= _effectsArrange->getModifiers().size()) return;
        } 
        else {
            int impactRelativeIdx = selectedEffectIndex - maxModifiers;
            if (impactRelativeIdx >= _effectsArrange->getImpacts().size()) return;
        }
        

        isMovingEffect = true;
        selectedTargetIndex = selectedEffectIndex;
    } 
    else {
        if (selectedEffectIndex != selectedTargetIndex) {
            bool startIsMod = selectedEffectIndex < maxModifiers;
            bool targetIsMod = selectedTargetIndex < maxModifiers;
            cout << "Start is Modifier: " << startIsMod << ", Target is Modifier: " << targetIsMod << std::endl;

            if (startIsMod == targetIsMod) {
                _effectsArrange->swapEffects(selectedEffectIndex, selectedTargetIndex);
            } 
            else {
                std::cout << "No puedes mezclar Modificadores con Impactos" << std::endl;
            }
        }
        isMovingEffect = false;
    }
}

void Rearrange::closeRearrangeMenu() {
    isOpen = false;
}

EffectsArrange* Rearrange::getEffectsArrange() const {
    return _effectsArrange;
}

int Rearrange::getTotalSlots() const {
    if (!_effectsArrange) return 1;
    
    int modSlots = _effectsArrange->getMaxModifiers();
    
    int impactSlots = _effectsArrange->getMaxImpacts(); 
    
    return modSlots + impactSlots;
}