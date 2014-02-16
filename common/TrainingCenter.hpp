#ifndef TrainingCenter_hpp
#define TrainingCenter_hpp

#include "ImprovementBuilding.hpp"
#include "ManagedPlayer.hpp"

class TrainingCenter : public ImprovementBuilding {

 public:

    void train(ManagedPlayer& player, int capacityNumber);

/*
    void trainStrength(ManagedPlayer& player);

    void trainPrecision(ManagedPlayer& player);

    void trainReflex(ManagedPlayer& player);

    void trainResistance(ManagedPlayer& player);
*/

    int getPriceToTrainCaract(ManagedPlayer& player, int capacityNumber);
	void setPriceForNextlevel();

    void getInfos();
};

#endif // TrainingCenter_hpp
