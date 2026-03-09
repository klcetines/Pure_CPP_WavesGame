#include "Effects/ProjectileEffects/PiercingEffect.h"

EffectType PiercingEffect::GetType() const {
    return EffectType::Piercing; 
}

std::string PiercingEffect::getModifierSymbol() const {
    return "P"; 
}

uint32_t PiercingEffect::getColorCode() const {
    return 0xFFFF00; 
}

ProjectileAction PiercingEffect::OnImpact(IActor& enemy) {
    return ProjectileAction::Continue;
}

std::unique_ptr<IProjectileEffect> PiercingEffect::Clone() const {
    return std::make_unique<PiercingEffect>(*this);
}

bool PiercingEffect::extraImpact() const {
    return true;
}

bool PiercingEffect::isPassive() const {
    return true;
}