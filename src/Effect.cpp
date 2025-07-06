#include "Effect.h"

Effect::Effect(const string& type, float value){
    _type = type;
    _value = value;
}

string Effect:: getType() const{
    return _type;
}
float Effect::getValue() const{
    return _value;
}
void Effect::setType(const string& t) {
    _type = t;
}
void Effect::setValue(float v) {
    _value = v;
}