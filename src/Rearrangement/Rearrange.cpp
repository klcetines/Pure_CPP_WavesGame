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
            RearrangeRender::renderRearrangeMenu(*this, window);
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
    if (isMovingEffect) {
        selectedTargetIndex = (selectedTargetIndex + 1) % (_effectsArrange->getModifiers().size() + _effectsArrange->getImpacts().size());
    } 
    else {
        selectedEffectIndex = (selectedEffectIndex + 1) % (_effectsArrange->getModifiers().size() + _effectsArrange->getImpacts().size());
    }
}

void Rearrange::moveSelectionToPreviousEffect() {
    if (isMovingEffect) {
        selectedTargetIndex = (selectedTargetIndex - 1 + (_effectsArrange->getModifiers().size() + _effectsArrange->getImpacts().size())) % (_effectsArrange->getModifiers().size() + _effectsArrange->getImpacts().size());
    } 
    else {
        selectedEffectIndex = (selectedEffectIndex - 1 + (_effectsArrange->getModifiers().size() + _effectsArrange->getImpacts().size())) % (_effectsArrange->getModifiers().size() + _effectsArrange->getImpacts().size());
    }
}

void Rearrange::selectEffect() {
    if (!isMovingEffect) {
        isMovingEffect = true;
        selectedTargetIndex = selectedEffectIndex;
    } 
    else {
        if (selectedEffectIndex != selectedTargetIndex) {
            _effectsArrange->swapEffects(selectedEffectIndex, selectedTargetIndex);
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