
#include "Player.hpp"
#include "Broomstick.hpp"

#include <string>

using namespace std;

Player::Player(string playerSaveFile) {}

string Player::getFirstName() {return _firstName;}
string Player::getLastName() {return _lastName;}
int Player::getCapacity(int capacityNumber) {return _capacities[capacityNumber];}

int Player::getLife() {return _life;}
Broomstick Player::getBroomstick() {return _broomstick;}

void Player::setFirstName(string firstName) {_firstName = firstName;}
void Player::setLastName(string lastName) {_lastName = lastName;}
void Player::up(int capacityNumber) {_capacities[capacityNumber]+=1;}
void Player::setLife(int life) {_life = life;}
void Player::setBroomstick(Broomstick broomstick) {_broomstick = broomstick;}