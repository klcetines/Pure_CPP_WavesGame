#ifndef PROJECTILE_EFFECT_LIST_H
#define PROJECTILE_EFFECT_LIST_H

#include "Projectiles/ProjectileEffect.h"

struct Node {
    ProjectileEffect* effect;
    Node* next;
    
    Node(ProjectileEffect* effect) : effect(effect), next(nullptr) {}
};

class ProjectileEffectsList
{
public:
    ProjectileEffectsList();
    ProjectileEffectsList(const ProjectileEffectsList&);
    ~ProjectileEffectsList();
    
    void pushBack(ProjectileEffect* effect);
    void removeEffect();

    Node* getFirst() const;

    Node* _current = nullptr;

private:
    Node* first;
};

#endif // PROJECTILE_EFFECT_LIST_H
