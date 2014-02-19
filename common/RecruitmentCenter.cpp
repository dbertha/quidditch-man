#include <iostream>

#include "Building.hpp"
#include "RecruitmentCenter.hpp"

typedef int gold;

using namespace std;

RecruitmentCenter::RecruitmentCenter(int level, gold price): Building(level,price) {}

gold RecruitmentCenter::getPriceForNextLevel(){
	int priceScale = 2*_priceForConstruction/5;
	gold priceForNextLevel = _priceForConstruction + priceScale*_level;

	return priceForNextLevel;
}

void RecruitmentCenter::displayInformations() {
	cout<<"----------- RECRUITMENT CENTER -----------"<<endl;
	cout<<"Level : "<<this->_level<<endl;
	cout<<"Price for next level : "<<this->getPriceForNextLevel()<<endl;
}