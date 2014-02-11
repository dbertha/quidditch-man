
#include "Player.hpp"
#include "Broomstick.hpp"

#include <fstream>
#include <string> 
#include <iostream>
#include <stdlib.h>     /* atoi */

using namespace std;

Player::Player(string& playerSaveFile) {
	ifstream file(playerSaveFile.c_str());
	if (!file) {
		cerr<<"Error while opening file\n";
		return;
	}
	getline(file,_firstName);
	getline(file,_lastName);

	string tmp;
	for (int i=0;i<5;++i){
		getline(file,tmp);
		_capacities[i] = atoi(tmp.c_str());
	}

	getline(file,tmp);
	_popularity = atoi(tmp.c_str());

	getline(file,tmp);
	_blocked = atoi(tmp.c_str());

	file.close();
}

string Player::getFirstName() {return _firstName;}
string Player::getLastName() {return _lastName;}
int Player::getCapacity(int capacityNumber) {return _capacities[capacityNumber];}

int Player::getPopularity() {return _popularity;}
void Player::setPopularity(int popularity) {_popularity = popularity;}

void Player::lockPlayer() {_blocked = true;}
void Player::unlockPlayer() {_blocked = false;}
bool Player::isBlocked() {return _blocked;}

int Player::getLife() {return _life;}
Broomstick Player::getBroomstick() {return _broomstick;}

void Player::setFirstName(string firstName) {_firstName = firstName;}
void Player::setLastName(string lastName) {_lastName = lastName;}
void Player::up(int capacityNumber) {_capacities[capacityNumber]+=1;}

void Player::setLife(int life) {_life = life;}
void Player::setBroomstick(Broomstick broomstick) {_broomstick = broomstick;}
void Player::gainPopularity() {}