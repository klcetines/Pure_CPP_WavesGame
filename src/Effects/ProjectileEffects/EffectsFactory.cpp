#include "Effects/ProjectileEffects/EffectsFactory.h"
using namespace sf;
using namespace std;

EffectsFactory& EffectsFactory::Instance() {
    static EffectsFactory instance;
    return instance;
}

void EffectsFactory::Initialize(){
    Instance().RegisterEffect(0, []() { return std::make_unique<PiercingEffect>(); });
    Instance().RegisterEffect(1, []() { return std::make_unique<HomingEffect>(); });
    Instance().RegisterEffect(2, []() { return std::make_unique<FireProjectileEffect>(); });
    Instance().RegisterEffect(3, []() { return std::make_unique<BleedProjectileEffect>(); });
    Instance().RegisterEffect(4, []() { return std::make_unique<SpinningProjectileEffect>(); });
    Instance().RegisterEffect(5, []() { return std::make_unique<LaserEffect>(); });
    Instance().RegisterEffect(6, []() { return std::make_unique<ShotgunEffect>(); });
    Instance().RegisterEffect(7, []() { return std::make_unique<IceProjectileEffect>(); });
}

void EffectsFactory::RegisterEffect(const int id, CreatorFunc func) {
    creators[id] = func;
}

std::unique_ptr<IProjectileEffect> EffectsFactory::Create(const int id) {
    auto it = creators.find(id);
    if (it != creators.end()) {
        return it->second();
    }
    else{
        std::cout << "EffectsFactory: Unknown effect id '" << id << "'" << std::endl;
    }
    return nullptr;
}