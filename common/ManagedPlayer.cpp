#include <string>

#include "Player.hpp"
#include "Broomstick.hpp"

#include "ManagedPlayer.hpp"

using namespace std;

ManagedPlayer::ManagedPlayer(string playerSaveFile): Player(playerSaveFile) {}

int ManagedPlayer::getPopularity() {return _popularity;}
void ManagedPlayer::setPopularity(int popularity) {_popularity = popularity;}

void ManagedPlayer::lockPlayer() {_blocked = true;}
void ManagedPlayer::unlockPlayer() {_blocked = false;}
bool ManagedPlayer::isBlocked() {return _blocked;}

void ManagedPlayer::train(int capacityNumber){
	_trainingLeft[capacityNumber] -= 1;
	if (_trainingLeft[capacityNumber] == 0) {
		this->up(capacityNumber);
		_trainingLeft[capacityNumber] = this->getCapacity(capacityNumber)-1;
	}
}
void ManagedPlayer::updateLife() {}
void ManagedPlayer::gainPopularity() {}
int ManagedPlayer::getEstimatedValue() {return 0;}

void ManagedPlayer::heal() {_life += 1;}
