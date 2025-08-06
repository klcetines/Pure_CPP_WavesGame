#ifndef EFFECT_H
#define EFFECT_H

#include <string>
#include <memory>
#include "Character/Character.h"

using namespace std;

class Effect {
public:
    Effect();
    Effect(const string& type, float value);
    string getType() const;
    float getValue() const;
    void setType(const string& t);
    void setValue(float v);

private:
    string _type;
    float _value;
};

#endif