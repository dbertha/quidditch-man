#ifndef RecruitmentCenter_hpp
#define RecruitmentCenter_hpp

#include "Building.hpp"
//#include "Manager.hpp"
//#include "ManagedPlayer.hpp"

typedef int gold;

class RecruitmentCenter : public Building {	

 public:

 	RecruitmentCenter(int level=0, gold price=0);

    gold getPriceForNextLevel();
    //void buyPlayer(Manager& manager, ManagedPlayer& player, gold price);
    //void sellPlayer(Manager& manager, ManagedPlayer& player, gold price);

    void displayInformations();
    //void getInfos();

};

#endif //RecruitmentCenter_hpp
