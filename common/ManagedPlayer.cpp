#include <string>

#include "Player.hpp"
#include "Broomstick.hpp"

#include "ManagedPlayer.hpp"

using namespace std;

ManagedPlayer::ManagedPlayer(string playerSaveFile): Player(playerSaveFile) {
	for (int i=0;i<5;++i){
		_trainingLeft[i] = this->getCapacity(i) - 1;
	}
}

void ManagedPlayer::train(int capacityNumber){
	_trainingLeft[capacityNumber] -= 1;
	if (_trainingLeft[capacityNumber] == 0) {
		this->up(capacityNumber);
		_trainingLeft[capacityNumber] = this->getCapacity(capacityNumber)-1;
	}
}
void ManagedPlayer::updateLife() {}
int ManagedPlayer::getEstimatedValue() {return 0;}

void ManagedPlayer::heal() {this->setLife(this->getLife()+1);}
