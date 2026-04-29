#include "Utils/SelfRNG.h"

SelfRNG::SelfRNG(unsigned int seed) : engine(seed) {

}
        
double SelfRNG::f() { 
    return uniform01(engine); 
}

int SelfRNG::i(int a, int b) {
    std::uniform_int_distribution<> dist(a, b);
    return dist(engine);
}

bool SelfRNG::bernoulli(double p) {
    std::bernoulli_distribution dist(p);
    return dist(engine);
}

int SelfRNG::poisson(double mean) {
    std::poisson_distribution<> dist(mean);
    return dist(engine);
}