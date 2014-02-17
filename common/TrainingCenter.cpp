

#include "ImprovementBuilding.hpp"
#include "TrainingCenter.hpp"
#include "ManagedPlayer.hpp"

#define TIMESCALE 1 //In minute

typedef int gold;

TrainingCenter::TrainingCenter(int level, gold price, int timeRequired): ImprovementBuilding(level,price,timeRequired) {}

void TrainingCenter::train(ManagedPlayer& player, int capacityNumber) {
	player.lockPlayer();
	player.train(capacityNumber);
}