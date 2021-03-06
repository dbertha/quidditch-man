#ifndef FanShop_hpp
#define FanShop_hpp

#include "Building.hpp"

#include <vector>

typedef int gold;

class FanShop : public Building {

	int _maxClientsAtFirstLevel;

 public:

 	FanShop(int level=0, gold price=0, int maxClients=0, bool isUpgrading=false);

    gold getIncome();
    int getMaxClients();
    int getMaxClientsAtFirstLevel();
    gold getPriceForNextLevel();
    
    std::vector<int> getInformations();
};

#endif //FanShop_hpp
