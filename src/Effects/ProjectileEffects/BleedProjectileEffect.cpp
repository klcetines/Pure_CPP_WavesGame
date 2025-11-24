#include "Effects/ProjectileEffects/BleedProjectileEffect.h"
#include "Effects/CharacterEffects/ActorEffectComponent.h"

EffectType BleedProjectileEffect::GetType() const {
    return EffectType::Impact; 
}

ProjectileAction BleedProjectileEffect::OnImpact(IActor& enemy) {
    ActorEffectComponent* effectComp = enemy.getEffectComponent();
    if (!effectComp) return ProjectileAction::Destroy;

    if (auto existingEffect = effectComp->get(ActorEffectType::Bleed)) {
        existingEffect->refreshDuration();
        existingEffect->addCharges(1);
    } 
    else {
        auto bleedEffect = ActorEffectsFactory::Instance().Create(3);
        effectComp->AddEffect(std::move(bleedEffect));
    }

    return ProjectileAction::Trigger;
}

std::unique_ptr<IProjectileEffect> BleedProjectileEffect::Clone() const {
    return std::make_unique<BleedProjectileEffect>(*this);
};