#include <iostream>

#include <cmath>

#include "Building.hpp"
#include "Stadium.hpp"
#include <vector>

typedef int gold;

using namespace std;

Stadium::Stadium(int level, gold price, int maxPlaces): Building(level,price), _maxPlacesAtFirstLevel(maxPlaces) {}

gold Stadium::getPriceForNextLevel() {
	int priceScale = _priceForConstruction/2;
	int priceForNextLevel = this->_priceForConstruction + priceScale*this->_level;

	return priceForNextLevel;
}
//Number of max places in the stadium is multiplied by 2 with every upgrade.
int Stadium::getMaxPlaces() {
	int maxPlaces =  _maxPlacesAtFirstLevel * pow(2,(this->_level)-1);
	return maxPlaces;
}
int Stadium::getMaxPlacesAtFirstLevel() {return _maxPlacesAtFirstLevel;}

vector<int> Stadium::getInformations() {
	/*cout<<"----------- STADIUM -----------"<<endl;
	cout<<"Level : "<<this->_level<<endl;
	cout<<"Price for next level : "<<this->getPriceForNextLevel()<<endl;
	cout<<"Max places : "<<this->getMaxPlaces()<<endl;*/

	vector<int> tmp;
	tmp.push_back(this->getLevel());
	tmp.push_back(this->getPriceForNextLevel());
	tmp.push_back(this->getMaxPlaces());

	return tmp;
}
