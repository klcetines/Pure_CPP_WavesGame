#ifndef GAMESTATISTICS_H
#define GAMESTATISTICS_H

class GameStatistics {
public:
    GameStatistics();

    void addKill();
    void addProjectile();
    bool spendCurrency(int amount);

    int getEnemiesKilled() const;
    int getProjectilesFired() const;
    int getCurrency() const;

private:
    int enemiesKilled;
    int projectilesFired;
    int currency;
};

#endif