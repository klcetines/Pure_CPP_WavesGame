#ifndef Self_RNG_H
#define Self_RNG_H

class SelfRNG {
    private:
        std::mt19937 engine;
        std::uniform_real_distribution<> uniform01{0.0, 1.0};
        std::uniform_int_distribution<> uniformInt;
        
    public:
        RNG(unsigned int seed = 1) : engine(seed) {}
        
        double f();
        
        int i(int a, int b);
        
        template<typename T>
        T pick(const std::vector<T>& a);
        
        template<typename T>
        void sh(std::vector<T>& a);
        
        bool bernoulli(double p = 0.5);
        
        int poisson(double mean);
};

#endif // Self_RNG_H