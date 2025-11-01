#include "Effects/ProjectileEffects/FireProjectileEffect.h"
#include "Effects/CharacterEffects/ActorEffectComponent.h"

ProjectileAction FireProjectileEffect::OnImpact(IActor& enemy) {
    ActorEffectComponent* effectComp = enemy.getEffectComponent();
        if (effectComp) {
            auto fireEffect = ActorEffectsFactory::Instance().Create(2);
            effectComp->AddEffect(std::move(fireEffect));
        }
    return ProjectileAction::Trigger;
}

std::unique_ptr<IProjectileEffect> FireProjectileEffect::Clone() const {
    return std::make_unique<FireProjectileEffect>(*this);
};