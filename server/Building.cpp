

#include "Building.hpp"

typedef int gold;

Building::Building(int lvl, gold price, bool isUpgrading):_level(lvl), _priceForConstruction(price), _isUpgrading(isUpgrading) {}

void Building::upgrade(){
	++_level;
	_isUpgrading=false;
}
int Building::getLevel(){return _level;}
gold Building::getPriceForConstruction() {return _priceForConstruction;}
bool Building::isUpgrading() {return _isUpgrading;}
void Building::startConstruction() {_isUpgrading=true;}

