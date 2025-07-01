#ifndef LIFE_H
#define LIFE_H

class Life {
    public:
        Life(float maxLife);
        ~Life() = default;

        void takeDamage(float amount);
        void heal(float amount);
        float getLife() const;
        float getMaxLife() const;
        bool isAlive() const;
        void setLife(float value);

    private:
        float life;
        float maxLife;
};

#endif // LIFE_H