#ifndef PROJECTILE_EFFECT_H
#define PROJECTILE_EFFECT_H

#include <string>

using namespace std;

namespace ProjectileEffects {
    constexpr int PIERCING   = 0;
    constexpr int BLEED      = 1;
    constexpr int POISON     = 2;
    constexpr int FIRE       = 3;
    constexpr int ICE        = 4;
    constexpr int AUTOAIM    = 5;
    constexpr int BARREL     = 6;
    constexpr int EXPLOSIVE  = 7;
    constexpr int DIVIDENT   = 8;
}

class ProjectileEffect{
    public:
        ProjectileEffect();
        ProjectileEffect(const int effects);
        ~ProjectileEffect() = default;
        bool checkEffect(string index);
        char getTriggeredBy() const;

    private:
        bool effects[9];
        char triggeredBy;
        void setTriggeredByValue(const int value);
};

#endif // PROJECTILE_EFFECT_H