#include "ImprovementBuilding.hpp"
#include "ManagedPlayer.hpp"

#define SPEED 0
#define STRENGTH 1
#define PRECISION 2
#define REFLEX 3
#define RESISTANCE 4


void TrainingCenter::trainSpeed(ManagedPlayer& player){
	player.lockPlayer();
	player.setTrainingLeft(SPEED,1);
	
	
	}

void TrainingCenter::getPriceToTrainCaract(ManagedPlayer& player){
	
	
	
	
	
	}

/*
class TrainingCenter : public ImprovementBuilding {

 public:

    void trainSpeed(ManagedPlayer& player);

    void trainStrength(ManagedPlayer& player);

    void trainPrecision(ManagedPlayer& player);

    void trainReflex(ManagedPlayer& player);

    void trainResistance(ManagedPlayer& player);

    int getPriceToTrainCaract(ManagedPlayer& player);

    int getPriceForNextLevel();

    void getInfos();
};*/

#endif // TrainingCenter_hpp
