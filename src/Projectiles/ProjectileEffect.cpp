#include "Projectiles/ProjectileEffect.h"

ProjectileEffect::ProjectileEffect() {
    for(int i = 0; i < 9; i++) {
        effects[i] = false;
    }
}

ProjectileEffect::ProjectileEffect(const int effects[9]){
    for(int i = 0; i < 9; i++) {
        this->effects[i] = false;
    }
    
    for(int i = 0; i < 9; i++) {
        if(effects[i] != 0) {
            this->effects[i] = true;
        }
    }
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
