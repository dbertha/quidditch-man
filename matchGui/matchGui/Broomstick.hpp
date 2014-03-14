#ifndef Broomstick_hpp
#define Broomstick_hpp

typedef int gold;

class Broomstick {

 public:

 	Broomstick(int capacity=0, int bonus=0);

    int _capacity;
    int _bonus;
    gold _value;
    
    int getCapacityBoosted();
    int getBonus();
    gold getValue();

};

#endif //Broomstick_hpp
