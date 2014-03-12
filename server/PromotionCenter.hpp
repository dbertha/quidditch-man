#ifndef PromotionCenter_hpp
#define PromotionCenter_hpp

#include "Building.hpp"
#include <vector>

typedef int gold;

class PromotionCenter : public Building {	

	int _gainAtFirstLevel;

 public:

 	PromotionCenter(int level=0, gold price=0, int gain=0, bool isUpgrading=false);

    gold getPriceForNextLevel();
    
    int getGainAtFirstLevel();
    int getActionPointsGain();

    std::vector<int> getInformations();

};

#endif //PromotionCenter_hpp
