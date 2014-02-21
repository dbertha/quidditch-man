
#include "Player.hpp"
#include "ManagedPlayer.hpp"
#include "Broomstick.hpp"

#include "PlayingPlayer.hpp"

#include <string> 
#include <iostream>
#include <stdlib.h>     /* atoi */
#include <ctime>
#include <cstdlib> 
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

PlayingPlayer::PlayingPlayer(ManagedPlayer& player) {
	Broomstick broomstick = player.getBroomstick();
	int tmp;
	for (int i=0;i<5;++i){
		tmp = player.getCapacity(i);
		if (i == broomstick.getCapacityBoosted()) tmp += broomstick.getBonus();
		this->setCapacity(i,tmp);
	}
}