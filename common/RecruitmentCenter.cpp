#include <iostream>

#include "Building.hpp"
#include "ManagedPlayer.hpp"
#include "RecruitmentCenter.hpp"
#include "Manager.hpp"

typedef int gold;

using namespace std;

RecruitmentCenter::RecruitmentCenter(int level, gold price): Building(level,price) {}

gold RecruitmentCenter::getPriceForNextLevel(){
	int priceScale = 2*_priceForConstruction/5;
	gold priceForNextLevel = _priceForConstruction + priceScale*_level;

	return priceForNextLevel;
}

void RecruitmentCenter::buyPlayer(Manager& manager, ManagedPlayer& player, gold price){
	manager.pay(price);
	manager.addPlayer(player);
}

void RecruitmentCenter::sellPlayer(Manager& manager, ManagedPlayer& player, gold price){
	try {

		manager.addMoney(price);
		manager.removePlayer(player);
	} catch (const char err[]) {cout<<err<<endl;}
}
