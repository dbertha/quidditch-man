#include <cmath>
#include <iostream>

#include "Building.hpp"
#include "FanShop.hpp"

#define SELLINGPRICE 50 //price of item sold by the fan shop

typedef int gold;

using namespace std;

FanShop::FanShop(int level, gold price, int maxClients): Building(level,price), _maxClientsAtFirstLevel(maxClients) {}

gold FanShop::getIncome() {
	gold amount = SELLINGPRICE * this->getMaxClients();
	return amount;
}
//Number of max clients is multiplied by 2 each time the building is upgraded
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