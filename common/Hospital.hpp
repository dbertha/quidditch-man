#ifndef Hospital_hpp
#define Hospital_hpp


#include "ImprovementBuilding.hpp"
#include "ManagedPlayer.hpp"

typedef int gold;

class Hospital : public ImprovementBuilding {

 public:

 	Hospital(int level, gold price, int _timeRequired);

    void heal(ManagedPlayer& player);

};

#endif // Hospital_hpp
