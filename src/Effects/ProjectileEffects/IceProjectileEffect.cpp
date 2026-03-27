#include "Effects/ProjectileEffects/IceProjectileEffect.h"
#include "Effects/CharacterEffects/ActorEffectComponent.h"

EffectType IceProjectileEffect::GetType() const {
    return EffectType::Impact; 
}

std::string IceProjectileEffect::getModifierSymbol() const {
    return "Ice"; 
}

uint32_t IceProjectileEffect::getColorCode() const {
    return 0xFF333399; 
}

ProjectileAction IceProjectileEffect::OnImpact(IActor& enemy) {
    ActorEffectComponent* effectComp = enemy.getEffectComponent();
    if (!effectComp) return ProjectileAction::Destroy;

    if (auto existingEffect = effectComp->get(ActorEffectType::Ice)) {
        existingEffect->refreshDuration();
        existingEffect->addCharges(1);
    } 
    else {
        auto iceEffect = ActorEffectsFactory::Instance().Create(7);
        effectComp->AddEffect(std::move(iceEffect));
    }

    return ProjectileAction::Continue;
}

std::unique_ptr<IProjectileEffect> IceProjectileEffect::Clone() const {
    return std::make_unique<IceProjectileEffect>(*this);
};