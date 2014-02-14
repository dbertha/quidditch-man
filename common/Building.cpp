#include <iostream> // cin, cout...

#include "Building.hpp"

using namespace std;

//ManagedPlayer::ManagedPlayer(string playerSaveFile): Player(playerSaveFile) {}
/*
Building::Building( ){
	cout<<"test debil"<<endl;
	}
*/
Building::Building(int lvl):_level(lvl), _priceForNextLevel(lvl+1) {
	cout<<"*** Batiment cree"<<endl;
	cout<<"*** lvl = "<<_level<<endl;
	cout<<"*** prix prochain lvl = "<<_priceForNextLevel<<endl;
	}

void Building::upgradeLevel(){
	++ _level;
	++ _priceForNextLevel;
}

int Building::getPriceForNextLevel(){
	return _priceForNextLevel;
	

}
