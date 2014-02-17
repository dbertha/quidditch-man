

#include "Building.hpp"

typedef int gold;

Building::Building(int lvl, gold price):_level(lvl), _priceForConstruction(price) {}

void Building::upgradeBuilding(){++_level;}

int Building::getLevel(){return _level;}

