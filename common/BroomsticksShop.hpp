#ifndef BroomsticksShop_hpp
#define BroomsticksShop_hpp

#include <vector>

#include "ShoppingBuilding.hpp"
#include "Broomstick.hpp"

typedef int gold;

class BroomsticksShop : public ShoppingBuilding {

	vector<int> _bonusMax;

 public:

    void buyBroomstick();
    void sellBroomstick();

    gold getPriceForNextLevel();
    int getBonusMax();
    void getInfos();

};

#endif // BroomsticksShop_hpp
