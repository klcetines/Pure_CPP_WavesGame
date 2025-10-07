#include "Projectiles/ProjectileEffect.h"

ProjectileEffect::ProjectileEffect() {
    for(int i = 0; i < 9; i++) {
        effects[i] = false;
    }
}

ProjectileEffect::ProjectileEffect(const int value){
    for(int i = 0; i < 9; i++) {
        this->effects[i] = false;
    }
    
    effects[value] = true;
}

bool ProjectileEffect::checkEffect(string index){
    if(index == "PIERCING"){
        return effects[ProjectileEffects::PIERCING];
    }
    else if(index == "BLEED"){
        return effects[ProjectileEffects::BLEED];
    }
    else if(index == "POISON"){
        return effects[ProjectileEffects::POISON];
    }
    else if(index == "FIRE"){
        return effects[ProjectileEffects::FIRE];
    }
    else if(index == "ICE"){
        return effects[ProjectileEffects::ICE];
    }
    else if(index == "AUTOAIM"){
        return effects[ProjectileEffects::AUTOAIM];
    }
    else if(index == "BARREL"){
        return effects[ProjectileEffects::BARREL];
    }
    else if(index == "EXPLOSIVE"){
        return effects[ProjectileEffects::EXPLOSIVE];
    }
    else if(index == "DIVIDENT"){
        return effects[ProjectileEffects::DIVIDENT];
    }
    return false;
}

char ProjectileEffect::getTriggeredBy() const {
    return triggeredBy;
}

void ProjectileEffect::setTriggeredByValue(const int value) {
    switch (value) {
        case ProjectileEffects::PIERCING:
        case ProjectileEffects::BLEED:
        case ProjectileEffects::POISON:
        case ProjectileEffects::FIRE:
        case ProjectileEffects::ICE:
        case ProjectileEffects::EXPLOSIVE:
            triggeredBy = 'I'; 
            break;
        case ProjectileEffects::AUTOAIM:
        case ProjectileEffects::BARREL:
            triggeredBy = 'A'; 
            break;
        case ProjectileEffects::DIVIDENT:
            triggeredBy = 'D'; 
            break;
        default:
            triggeredBy = 'A'; 
            break;
    }
}

