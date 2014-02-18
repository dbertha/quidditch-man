#ifndef FanShop_hpp
#define FanShop_hpp

#include "Building.hpp"

typedef int gold;

class FanShop : public Building {

	int _maxClientsAtFirstLevel;

 public:

 	FanShop(int level=0, gold price=0, int maxClients=0);

    gold getIncome();
    int getMaxClients();
    int getMaxClientsAtFirstLevel();
    gold getPriceForNextLevel();
    
    void displayInformations();
    //void getInfos();
};

#endif //FanShop_hpp
