#ifndef RecruitmentCenter_hpp
#define RecruitmentCenter_hpp

#include "Building.hpp"
#include <vector>

typedef int gold;

class RecruitmentCenter : public Building {	

 public:

 	RecruitmentCenter(int level=0, gold price=0);

    gold getPriceForNextLevel();

    std::vector<int> getInformations();

};

#endif //RecruitmentCenter_hpp
