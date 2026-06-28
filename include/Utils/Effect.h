#ifndef EFFECT_H
#define EFFECT_H

#include <string>
#include <memory>


class Effect {
public:
    Effect();
    Effect(const std::string& type, float value);
    std::string getType() const;
    float getValue() const;
    void setType(const std::string& t);
    void setValue(float v);

private:
    std::string _type;
    float _value;
};

#endif