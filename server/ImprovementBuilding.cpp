#include <iostream>

#include "Building.hpp"
#include "ImprovementBuilding.hpp"

#include <vector>

#define TIMESCALE 1 //en minute

typedef int gold;

using namespace std;

ImprovementBuilding::ImprovementBuilding(int level, gold price, int timeRequired) : Building(level, price), _timeRequired(timeRequired) {}

gold ImprovementBuilding::getPriceForNextLevel() {
	int priceScale = _priceForConstruction/2;
	gold priceForNextLevel = _priceForConstruction + priceScale*_level;

	return priceForNextLevel;
}
//timeRequired is in fact just a number. At each level, timeRequired decreases by 1.
//The time required, in minutes, is timeRequired multiplied by TIMESCALE 
int ImprovementBuilding::getTimeRequired(){return TIMESCALE*(_timeRequired-(this->_level-1));}
int ImprovementBuilding::getTimeRequiredAtFirstLevel() {return _timeRequired;}

vector<int> ImprovementBuilding::getInformations() {
	/*cout<<"----------- IMPROVEMENT BUILDING -----------"<<endl;
	cout<<"Level : "<<this->_level<<endl;
	cout<<"Price for next level : "<<this->getPriceForNextLevel()<<endl;
	cout<<"Time required : "<<this->getTimeRequired()<<endl;*/

	vector<int> tmp;
	tmp.push_back(this->getLevel());
	tmp.push_back(this->getPriceForNextLevel());
	tmp.push_back(this->getTimeRequired());
	return tmp;
}