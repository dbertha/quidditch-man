#include <iostream>

#include "Building.hpp"
#include "ImprovementBuilding.hpp"

#define TIMESCALE 1 //en minute

typedef int gold;

using namespace std;

ImprovementBuilding::ImprovementBuilding(int level, gold price, int timeRequired) : Building(level, price), _timeRequired(timeRequired) {}

gold ImprovementBuilding::getPriceForNextLevel() {
	int priceScale = _priceForConstruction/2;
	gold priceForNextLevel = _priceForConstruction + priceScale*_level;

	return priceForNextLevel;
}

int ImprovementBuilding::getTimeRequired(){return TIMESCALE*(_timeRequired-(this->_level-1));}
int ImprovementBuilding::getTimeRequiredAtFirstLevel() {return _timeRequired;}

void ImprovementBuilding::displayInformations() {
	cout<<"----------- IMPROVEMENT BUILDING -----------"<<endl;
	cout<<"Level : "<<this->_level<<endl;
	cout<<"Price for next level : "<<this->getPriceForNextLevel()<<endl;
	cout<<"Time required : "<<this->getTimeRequired()<<endl;
}