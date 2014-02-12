
#include "Broomstick.hpp"

#define PRICESCALE 50000
typedef int gold;

Broomstick::Broomstick(int capacity, int bonus): _capacity(capacity), _bonus(bonus), _value(bonus*PRICESCALE) {}
    
int Broomstick::getCapacityBoosted() {return _capacity;}
int Broomstick::getBonus() {return _bonus;}
gold Broomstick::getValue() {return _value;}