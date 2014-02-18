#include <cmath>
#include <iostream>

#include "Building.hpp"
#include "FanShop.hpp"

#define SELLINGRICE 50

typedef int gold;

using namespace std;

FanShop::FanShop(int level, gold price, int maxClients): Building(level,price), _maxClientsAtFirstLevel(maxClients) {}

gold FanShop::getIncome() {
	gold amount = SELLINGRICE * this->getMaxClients();
	return amount;
}

int FanShop::getMaxClients() {return _maxClientsAtFirstLevel * pow(2,(this->_level)-1);}
int FanShop::getMaxClientsAtFirstLevel() {return _maxClientsAtFirstLevel;}

gold FanShop::getPriceForNextLevel() {
	int priceScale = _priceForConstruction/3;
	gold priceForNextLevel = _priceForConstruction + priceScale*_level;

	return priceForNextLevel;
}

void FanShop::displayInformations() {
	cout<<"----------- FANSHOP -----------"<<endl;
	cout<<"Level : "<<this->_level<<endl;
	cout<<"Price for next level : "<<this->getPriceForNextLevel()<<endl;
	cout<<"Max clients : "<<this->getMaxClients()<<endl;
}