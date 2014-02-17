
#include "Building.hpp"
#include "ImprovementBuilding.hpp"

#define TIMESCALE 1 //en minute

typedef int gold;

ImprovementBuilding::ImprovementBuilding(int level, gold price, int timeRequired) : Building(level, price), _timeRequired(timeRequired) {}

gold ImprovementBuilding::getPriceForNextLevel() {
	int priceScale = _priceForConstruction/2;
	gold priceForNextLevel = _priceForConstruction + priceScale*_level;

	return priceForNextLevel;
}

int ImprovementBuilding::getTimeRequired(){return TIMESCALE*(_timeRequired-(this->_level-1));}