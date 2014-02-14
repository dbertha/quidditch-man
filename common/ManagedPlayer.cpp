#include <string> 
#include <iostream>
#include <cstdlib> 
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include "Player.hpp"
#include "Broomstick.hpp"

#include "ManagedPlayer.hpp"

using namespace std;


#define PRICESCALE 1000
typedef int gold;

ManagedPlayer::ManagedPlayer(string playerSaveFile): Player(playerSaveFile) {
	int fd = open(playerSaveFile.c_str(),O_RDONLY);
	if (fd==-1) {
		cerr<<"Error while opening file\n";
		return;
	}
	lseek(fd,_offset,SEEK_SET);
	
	char buffer[100];
	int bytes;
	bytes = read(fd,buffer,sizeof(buffer));
	buffer[bytes]='\0';

	string tmp;
	tmp = strtok(buffer,"\n");
	_trainingLeft[0] = atoi(tmp.c_str());

	for (int i=1;i<5;++i){
		tmp = strtok(NULL,"\n");
		_trainingLeft[i] = atoi(tmp.c_str());
	}

	tmp = strtok(NULL,"\n");
	_popularity = atoi(tmp.c_str());

	tmp = strtok(NULL,"\n");
	_blocked = atoi(tmp.c_str());

	tmp = strtok(NULL,"\n");
	int broomstickCapacity = atoi(tmp.c_str());

	tmp = strtok(NULL,"\n");
	int broomstickBonus = atoi(tmp.c_str());

	_broomstick = Broomstick(broomstickCapacity,broomstickBonus);

	close(fd);
}

int ManagedPlayer::getTrainingLeft(int capacityNumber) {return _trainingLeft[capacityNumber];}
void ManagedPlayer::setTrainingLeft(int capacityNumber, int value) {_trainingLeft[capacityNumber] = value;}

int ManagedPlayer::getPopularity() {return _popularity;}
void ManagedPlayer::setPopularity(int popularity) {_popularity = popularity;}

void ManagedPlayer::lockPlayer() {_blocked = true;}
void ManagedPlayer::unlockPlayer() {_blocked = false;}
bool ManagedPlayer::isBlocked() {return _blocked;}

int ManagedPlayer::getLife() {return _life;}
Broomstick ManagedPlayer::getBroomstick() {return _broomstick;}


void ManagedPlayer::setLife(int life) {_life = life;}
void ManagedPlayer::setBroomstick(Broomstick broomstick) {_broomstick = broomstick;}

void ManagedPlayer::gainPopularity() {this->setPopularity(this->getPopularity()+this->getPopularity()/10);} //Augmentation de 10%
void ManagedPlayer::train(int capacityNumber){
	this->setTrainingLeft(capacityNumber,this->getTrainingLeft(capacityNumber)-1);
	if (this->getTrainingLeft(capacityNumber) == 0) {
		this->up(capacityNumber);
		this->setTrainingLeft(capacityNumber,this->getCapacity(capacityNumber)-1);
	}
}
void ManagedPlayer::updateLife() {}
gold ManagedPlayer::getEstimatedValue() {
	int index=0; //Déterminé par les capacités et l'avancement de leur entraînement
	for (int i=0;i<5;++i){
		index+=2*this->getCapacity(i) - _trainingLeft[i];
	}

	return index*PRICESCALE + _broomstick.getValue();
}

void ManagedPlayer::heal() {this->setLife(this->getLife()+1);}
