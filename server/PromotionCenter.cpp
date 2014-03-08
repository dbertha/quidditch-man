#include <iostream>

#include "Building.hpp"
#include "PromotionCenter.hpp"
#include <vector>

typedef int gold;

using namespace std;

PromotionCenter::PromotionCenter(int level, gold price, int gain, bool isUpgrading): Building(level,price, isUpgrading),_gainAtFirstLevel(gain) {}

gold PromotionCenter::getPriceForNextLevel(){
	int priceScale = 2*_priceForConstruction/5;
	gold priceForNextLevel = _priceForConstruction + priceScale*_level;

	return priceForNextLevel;
}

int PromotionCenter::getActionPointsGain(){
	int gain = _gainAtFirstLevel;
	for (int i=1;i<_level;++i) {
		gain*=2;
	}
	return gain;
}

vector<int> PromotionCenter::getInformations() {
	/*cout<<"----------- RECRUITMENT CENTER -----------"<<endl;
	cout<<"Level : "<<this->_level<<endl;
	cout<<"Price for next level : "<<this->getPriceForNextLevel()<<endl;*/

	vector<int> tmp;
	tmp.push_back(this->getLevel());
	tmp.push_back(this->getPriceForNextLevel());
	tmp.push_back(this->getActionPointsGain());
	tmp.push_back(this->isUpgrading());	

	return tmp;
}
