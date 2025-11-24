#include "Effects/ProjectileEffects/FireProjectileEffect.h"
#include "Effects/CharacterEffects/ActorEffectComponent.h"

EffectType FireProjectileEffect::GetType() const {
    return EffectType::Impact; 
}

ProjectileAction FireProjectileEffect::OnImpact(IActor& enemy) {
    ActorEffectComponent* effectComp = enemy.getEffectComponent();
    if (!effectComp) return ProjectileAction::Destroy;

    if (auto existingEffect = effectComp->get(ActorEffectType::Fire)) {
        existingEffect->refreshDuration();
        existingEffect->addCharges(1);
    } 
    else {
        auto fireEffect = ActorEffectsFactory::Instance().Create(2);
        effectComp->AddEffect(std::move(fireEffect));
    }

    return ProjectileAction::Trigger;
}

std::unique_ptr<IProjectileEffect> FireProjectileEffect::Clone() const {
    return std::make_unique<FireProjectileEffect>(*this);
};