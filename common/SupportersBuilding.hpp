#ifndef SupportersBuilding_hpp
#define SupportersBuilding_hpp

#include "Building.hpp"


class SupportersBuilding : public Building {

 protected:
    vector<int> _incomeBySupporter;

 public:
    virtual int getIncome()  = 0;
};

#endif //SupportersBuilding_hpp
