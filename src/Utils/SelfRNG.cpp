#include "Utils/SelfRNG.h"

SelfRNG::SelfRNG(unsigned int seed = 1);
        
double SelfRNG::f() { 
    return uniform01(engine); 
}

int SelfRNG::i(int a, int b) {
    std::uniform_int_distribution<> dist(a, b);
    return dist(engine);
}

template<typename T> T 
SelfRNG::pick(const std::vector<T>& a) {
    if (a.empty()) throw std::runtime_error("Cannot pick from empty vector");
    return a[i(0, a.size() - 1)];
}

template<typename T>
void SelfRNG::sh(std::vector<T>& a) {
    std::shuffle(a.begin(), a.end(), engine);
}

bool SelfRNG::bernoulli(double p = 0.5) {
    std::bernoulli_distribution dist(p);
    return dist(engine);
}

int SelfRNG::poisson(double mean) {
    std::poisson_distribution<> dist(mean);
    return dist(engine);
}