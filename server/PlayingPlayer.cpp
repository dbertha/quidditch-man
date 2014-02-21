
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
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


PlayingPlayer::PlayingPlayer(ManagedPlayer& player, int role, AxialCoordinates startingPosition) : __role(role), __currentPosition(startingPosition), __hasQuaffle(0){
	Broomstick broomstick = player.getBroomstick();
	int tmp;
	for (int i=0;i<5;++i){
		tmp = player.getCapacity(i);
		if (i == broomstick.getCapacityBoosted()) tmp += broomstick.getBonus();
		this->setCapacity(i,tmp);
	}
}

int PlayingPlayer::getMaxDistance(){
    return getCapacity(SPEED);
}

void PlayingPlayer::hurted(){
    setLife(getLife() - 1);
}

void PlayingPlayer::handleBludger(){
    srand(time(NULL));
    if((getCapacity(REFLEX) * (rand() % 10)) > 10){ //touché
        hurted();
    }
}

bool PlayingPlayer::interceptQuaffle(int distance){
    return (getCapacity(PRECISION) * getCapacity(REFLEX)) > (distance * 10);
}

bool PlayingPlayer::catchGoldenSnitch(int distance){
    return (getCapacity(REFLEX)) > (distance);
}

void PlayingPlayer::moveTo(AxialCoordinates destination){
    __currentPosition = destination;
}

void PlayingPlayer::loseQuaffle(){
    __hasQuaffle = false;
}
void PlayingPlayer::carryQuaffle(){
    __hasQuaffle = true;
}

char * PlayingPlayer::serializeAttributes(char * bufferPosition){
    int attribute;
    for(int i = 0; i < 5; ++i){
	attribute = getCapacity(i);
	memcpy(bufferPosition, &attribute, sizeof(attribute));
	bufferPosition += sizeof(attribute);
    }
    attribute = __hasQuaffle; //conversion to int
    memcpy(bufferPosition, &attribute, sizeof(attribute));
    bufferPosition += sizeof(attribute);
    return bufferPosition;
}


    


