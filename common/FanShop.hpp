#ifndef FanShop_hpp
#define FanShop_hpp

#include "Building.hpp"
#include "Manager.hpp"

typedef int gold;

class FanShop : public Building {

	int _maxClientsAtFirstLevel;

 public:

 	FanShop(int level, gold price, int maxClients)

    void getIncome(Manager& manager);
    int getMaxClients();
    gold getPriceForNextLevel();
    //void getInfos();
};

#endif // FanShop_hpp
