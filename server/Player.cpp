
#include "Player.hpp"
#include "Broomstick.hpp"

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

Player::Player() : _offset(0) { // default constructor will be called but such player won't be used
	for (int i=0;i<5;++i) _capacities[i]=0;
}

Player& Player::operator= (const Player& player){
	for (int i=0;i<5;++i) _capacities[i] = player.getCapacity(i);
	return *this;
}

string Player::getFirstName() const {return _firstName;}
string Player::getLastName() const {return _lastName;}

int Player::getCapacity(int capacityNumber) const {return _capacities[capacityNumber];}
void Player::setCapacity(int capacityNumber, int value) {_capacities[capacityNumber] = value;}

int Player::getLife() const {return _life;}
void Player::setLife(int value) {
	_life = value;
}
void Player::heal() {_life = _capacities[4];}

void Player::setFirstName(string firstName) {_firstName = firstName;}
void Player::setLastName(string lastName) {_lastName = lastName;}
void Player::up(int capacityNumber) {_capacities[capacityNumber]+=1;}
