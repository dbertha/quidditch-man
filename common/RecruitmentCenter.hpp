#ifndef RecruitmentCenter_hpp
#define RecruitmentCenter_hpp

#include "Building.hpp"

typedef int gold;

class RecruitmentCenter : public Building {	

 public:

 	RecruitmentCenter(int level=0, gold price=0);

    gold getPriceForNextLevel();

    void displayInformations();

};

#endif //RecruitmentCenter_hpp
