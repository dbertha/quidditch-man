#include <string>

#include "Player.hpp"
#include "Broomstick.hpp"

#include "ManagedPlayer.hpp"

using namespace std;


#define PRICESCALE 10000
typedef int gold;

ManagedPlayer::ManagedPlayer(string playerSaveFile): Player(playerSaveFile) {}

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

	}

	return index*PRICESCALE;
}

void ManagedPlayer::heal() {this->setLife(this->getLife()+1);}
