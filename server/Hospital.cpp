
#include "ImprovementBuilding.hpp"
#include "Hospital.hpp"
#include "ManagedPlayer.hpp"

#define TIMESCALE 1 //In minute

typedef int gold;

Hospital::Hospital(int level, gold price, int timeRequired, bool isUpgrading): ImprovementBuilding(level,price,timeRequired,isUpgrading) {}

void Hospital::heal(ManagedPlayer& player) {
	player.lockPlayer();
	player.heal();
}
