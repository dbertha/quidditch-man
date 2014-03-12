#ifndef ImprovementBuilding_hpp
#define ImprovementBuilding_hpp

#include "Building.hpp"

typedef int gold;

class ImprovementBuilding : public Building {

 protected:
	int _timeRequired;

 public:

 	ImprovementBuilding(int level=0, gold price=0, int timeRequired=0);
 	
 	gold getPriceForNextLevel();
 	int getTimeRequired();
 	int getTimeRequiredAtFirstLevel();

 	
    void displayInformations();
};

#endif //ImprovementBuilding_hpp