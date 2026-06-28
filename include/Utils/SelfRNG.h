#ifndef SELF_RNG_H
#define SELF_RNG_H
#include <random>
#include <vector>
#include <stdexcept>
#include <algorithm>

class SelfRNG {
    private:
        std::mt19937 engine;
        std::uniform_real_distribution<> uniform01{0.0, 1.0};
        std::uniform_int_distribution<> uniformInt;
        
    public:
        SelfRNG(unsigned int seed = 1);
        
        double f();
        
        int i(int a, int b);
        
        template<typename T>
        T pick(const std::vector<T>& a) {
            if (a.empty()) throw std::runtime_error("Cannot pick from empty vector");
            return a[i(0, a.size() - 1)];
        }
        
        template<typename T>
        void sh(std::vector<T>& a) {
            std::shuffle(a.begin(), a.end(), engine);
        }

        bool bernoulli(double p = 0.5);
        
        int poisson(double mean);
};

#endif // SELF_RNG_H