#ifndef TrainingCenter_hpp
#define TrainingCenter_hpp

#include "ImprovementBuilding.hpp"
#include "ManagedPlayer.hpp"

typedef int gold;

class TrainingCenter : public ImprovementBuilding {

 public:

 	TrainingCenter(int level=0, gold price=0, int time=0, bool isUpgrading=false);

    void train(ManagedPlayer& player, int capacityNumber);

};

#endif //TrainingCenter_hpp
