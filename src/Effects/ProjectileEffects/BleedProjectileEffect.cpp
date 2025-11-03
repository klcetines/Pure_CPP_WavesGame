#include "Effects/ProjectileEffects/BleedProjectileEffect.h"
#include "Effects/CharacterEffects/ActorEffectComponent.h"

ProjectileAction BleedProjectileEffect::OnImpact(IActor& enemy) {
    ActorEffectComponent* effectComp = enemy.getEffectComponent();
        if (effectComp) {
            auto bleedEffect = ActorEffectsFactory::Instance().Create(3);
            effectComp->AddEffect(std::move(bleedEffect));
        }
    return ProjectileAction::Trigger;
}

std::unique_ptr<IProjectileEffect> BleedProjectileEffect::Clone() const {
    return std::make_unique<BleedProjectileEffect>(*this);
};