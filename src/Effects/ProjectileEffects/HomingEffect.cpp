#include "Effects/ProjectileEffects/HomingEffect.h"

EffectType HomingEffect::GetType() const {
    return EffectType::Homing; 
}

std::string HomingEffect::getModifierSymbol() const {
    return "H"; 
}

uint32_t HomingEffect::getColorCode() const {
    return 0x00FFFF; 
}

ProjectileAction HomingEffect::OnImpact(IActor& enemy) {
    return ProjectileAction::Trigger;
}

std::unique_ptr<IProjectileEffect> HomingEffect::Clone() const {
    return std::make_unique<HomingEffect>(*this);
}

bool HomingEffect::isPassive() const {
    return true;
}