#include "Projectiles/ProjectileEffectsList.h"
ProjectileEffectsList::ProjectileEffectsList()
    : first(nullptr)
{}

ProjectileEffectsList::ProjectileEffectsList(const ProjectileEffectsList& other)
    : first(nullptr)
{
    Node* current = other.first;
    while (current != nullptr) {
        pushBack(new ProjectileEffect(*current->effect));
        current = current->next;
    }
}

ProjectileEffectsList::~ProjectileEffectsList() {
    while (first != nullptr) {
        removeEffect();
    }
}
void ProjectileEffectsList::pushBack(ProjectileEffect* effect) {
    Node* newNode = new Node(effect);
    if (first == nullptr) {
        first = newNode;
        first->next = nullptr;
    } else {
        Node* current = first;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void ProjectileEffectsList::removeEffect() {
    if (first == nullptr) return;

    Node* temp = first;
    first = first->next;
    delete temp->effect;
    delete temp;
}

Node* ProjectileEffectsList::getFirst() const {
    return first;
}