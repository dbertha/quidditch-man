#ifndef ImprovementBuilding_hpp
#define ImprovementBuilding_hpp

#include "Building.hpp"

typedef int gold;

class ImprovementBuilding : public Building {

 protected:
	const int _timeRequired;

 public:

 	ImprovementBuilding(int level, gold price, int timeRequired);
 	
 	gold getPriceForNextLevel();
 	int getTimeRequired();
};

#endif //ImprovementBuilding_hpp
