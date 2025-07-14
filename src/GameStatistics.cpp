#include "GameStatistics.h"

    GameStatistics::GameStatistics() : enemiesKilled(0), projectilesFired(0), currency(0) {}

    void GameStatistics::addKill() { 
        enemiesKilled++; 
        currency+=100; 
    }

    void GameStatistics::addProjectile() { 
        projectilesFired++; 
    }

    bool GameStatistics::spendCurrency(int amount) {
        if (currency >= amount) {
            currency -= amount;
            return true;
        }
        return false;
    }

    int GameStatistics::getEnemiesKilled() const { 
        return enemiesKilled;
    }
    
    int GameStatistics::getProjectilesFired() const { 
        return projectilesFired; 
    }
    
    int GameStatistics::getCurrency() const { 
        return currency; 
    }