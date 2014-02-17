#include <cmath>

#include "Building.hpp"
#include "Manager.hpp"
#include "FanShop.hpp"

#define SELLINGRICE 50

typedef int gold

FanShop::FanShop(int level, gold price, int maxClients) : Building(level,price), _maxClientsAtFirstLevel(maxClients) {}

void FanShop::getIncome(Manager& manager) {
	int amount = SELLINGRICE * this->getMaxClients();
	manager.addMoney(amount);
}

int FanShop::getMaxClients() {return _maxClientsAtFirstLevel * 2**((this->_level)-1);}

gold FanShop::getPriceForNextLevel() {
	int priceScale = _priceForConstruction/3;
	gold priceForNextLevel = _priceForConstruction + priceScale*_level;

	return priceForNextLevel;
}