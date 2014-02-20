#include <iostream>

#include "Building.hpp"
#include "RecruitmentCenter.hpp"
#include <vector>

typedef int gold;

using namespace std;

RecruitmentCenter::RecruitmentCenter(int level, gold price, bool isUpgrading): Building(level,price, isUpgrading) {}

gold RecruitmentCenter::getPriceForNextLevel(){
	int priceScale = 2*_priceForConstruction/5;
	gold priceForNextLevel = _priceForConstruction + priceScale*_level;

	return priceForNextLevel;
}

vector<int> RecruitmentCenter::getInformations() {
	/*cout<<"----------- RECRUITMENT CENTER -----------"<<endl;
	cout<<"Level : "<<this->_level<<endl;
	cout<<"Price for next level : "<<this->getPriceForNextLevel()<<endl;*/

	vector<int> tmp;
	tmp.push_back(this->getLevel());
	tmp.push_back(this->getPriceForNextLevel());
	tmp.push_back(this->isUpgrading());

	return tmp;
}