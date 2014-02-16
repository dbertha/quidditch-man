#include <iostream> // cin, cout...

#include "Building.hpp"

using namespace std;

//ManagedPlayer::ManagedPlayer(string playerSaveFile): Player(playerSaveFile) {}
/*
Building::Building( ){
	cout<<"test debil"<<endl;
	}
*/
Building::Building(int lvl):_level(lvl){
	this->setPriceForNextlevel();
	cout<<"*** Batiment cree"<<endl;
	cout<<"*** lvl = "<<_level<<endl;
	cout<<"*** prix prochain lvl = "<<_priceForNextLevel<<endl;
	}

void Building::upgradeLevel(){
	++ _level;
	this->setPriceForNextlevel();
//	++ _priceForNextLevel;
}

int Building::getPriceForNextLevel(){
	return _priceForNextLevel;
}

int Building::getLevel(){
	return _level;
	}

//a rendre virtual pur?
void Building::setPriceForNextlevel(){
	_priceForNextLevel=_level+1;
}
