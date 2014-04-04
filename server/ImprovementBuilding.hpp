#ifndef ImprovementBuilding_hpp
#define ImprovementBuilding_hpp

#include <vector>
#include "Building.hpp"

typedef int gold;

class ImprovementBuilding : public Building {

 protected:
	int _timeRequired;

 public:

 	ImprovementBuilding(int level=0, gold price=0, int timeRequired=0, bool isUpgrading=false);
 	
 	gold getPriceForNextLevel();
 	virtual int getTimeRequired();
 	virtual int getTimeRequiredAtFirstLevel();

 	
    std::vector<int> getInformations();
};

#endif //ImprovementBuilding_hpp
