#ifndef Hospital_hpp
#define Hospital_hpp


#include "ImprovementBuilding.hpp"
#include "ManagedPlayer.hpp"

typedef int gold;

class Hospital : public ImprovementBuilding {

 public:

 	Hospital(int level=0, gold price=0, int _timeRequired=0, bool isUpgrading=false);

    void heal(ManagedPlayer& player);

};

#endif // Hospital_hpp
