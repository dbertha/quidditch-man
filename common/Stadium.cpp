
#include <cmath>

#include "Building.hpp"
#include "Stadium.hpp"

typedef int gold;

Stadium::Stadium(int level, int price, int maxPlaces): Building(level,price), _maxPlacesAtFirstLevel(maxPlaces) {}

gold Stadium::getPriceForNextLevel() {
	int priceScale = _priceForConstruction/2;
	int priceForNextLevel = this->_priceForConstruction + priceScale*this->_level;

	return priceForNextLevel;
}

int Stadium::getMaxPlaces() {
	int maxPlaces =  _maxPlacesAtFirstLevel * pow(2,(this->_level)-1);
	return maxPlaces;
}


