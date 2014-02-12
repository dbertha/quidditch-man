
#include "Player.hpp"
#include "Broomstick.hpp"

#include <fstream>
#include <string> 
#include <iostream>
#include <stdlib.h>     /* atoi */
#include <ctime>
#include <cstdlib> 

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

	for (int i=0;i<5;++i){
		getline(file,tmp);
		_trainingLeft[i] = atoi(tmp.c_str());
	}

	getline(file,tmp);
	_popularity = atoi(tmp.c_str());

	getline(file,tmp);
	_blocked = atoi(tmp.c_str());

	getline(file,tmp);
	int broomstickCapacity = atoi(tmp.c_str());

	getline(file,tmp);
	int broomstickBonus = atoi(tmp.c_str());

	_broomstick = Broomstick(broomstickCapacity,broomstickBonus);

	file.close();

	if ((_firstName=="John")&&(_lastName=="Doe")) verifyName();
}

string Player::getFirstName() {return _firstName;}
string Player::getLastName() {return _lastName;}
void Player::verifyName() {
	srand(time(0));
	do {
		int firstNameIndex, lastNameIndex;
		firstNameIndex = rand()%100 +1;
		lastNameIndex = rand()%100 +1;

		string tmp;
		ifstream file("firstNames.txt");
		for (int i=0;i<firstNameIndex;++i) {
			getline(file,tmp);
		}
		file.close();
		_firstName = tmp;
		file.open("lastNames.txt");
		for (int i=0;i<lastNameIndex;++i) {
			getline(file,tmp);
		}
		_lastName = tmp;
		file.close();
	} while (isNameTaken());
	ofstream file("namesTaken.txt",fstream::app);
	file<<_firstName;
	file<<"\n";
	file<<_lastName;
	file<<"\n";
	file.close();
}
bool Player::isNameTaken(){
	ifstream file("namesTaken.txt");
	if (!file) return 0;
	string first, last;
	while (getline(file,first)&&getline(file,last)){
		if ((_firstName==first)&&(_lastName==last)) {
			file.close();
			return 1;
		}
	}
	file.close();
	return 0;
}

int Player::getCapacity(int capacityNumber) {return _capacities[capacityNumber];}
int Player::getTrainingLeft(int capacityNumber) {return _trainingLeft[capacityNumber];}
void Player::setTrainingLeft(int capacityNumber, int value) {_trainingLeft[capacityNumber] = value;}

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
void Player::gainPopularity() {this->setPopularity(this->getPopularity()+this->getPopularity()/10);} //Augmentation de 10%