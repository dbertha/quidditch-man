#include <string> 
#include <iostream>
#include <cstdlib> 
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>


#include "Player.hpp"
#include "Broomstick.hpp"

#include "ManagedPlayer.hpp"

using namespace std;


#define PRICESCALE 1000
typedef int gold;

ManagedPlayer::ManagedPlayer() : _blocked(0) { //default constructor. This ManagedPlayer will never be used
	for (int i=0;i<5;++i) _trainingLeft[i] = 0;
	_broomstick = Broomstick(0,0);
}

ManagedPlayer& ManagedPlayer::operator= (const ManagedPlayer& player) {
	for (int i=0;i<5;++i) {
		this->setCapacity(i,player.getCapacity(i));
		_trainingLeft[i] = player.getTrainingLeft(i);
	}
	this->setFirstName(player.getFirstName());
	this->setLastName(player.getLastName());
	cout<<this->getFirstName()<<" "<<this->getLastName()<<endl;
	_blocked = player.isBlocked();
	this->setLife(player.getLife());
	return *this;
}

int ManagedPlayer::getTrainingLeft(int capacityNumber) const {return _trainingLeft[capacityNumber];}
void ManagedPlayer::setTrainingLeft(int capacityNumber, int value) {_trainingLeft[capacityNumber] = value;}

void ManagedPlayer::lockPlayer() {_blocked = true;}
void ManagedPlayer::unlockPlayer() {_blocked = false;}
bool ManagedPlayer::isBlocked() const {return _blocked;}

Broomstick ManagedPlayer::getBroomstick() {return _broomstick;}

void ManagedPlayer::setBroomstick(Broomstick broomstick) {_broomstick = broomstick;}

void ManagedPlayer::train(int capacityNumber){
	this->setTrainingLeft(capacityNumber,this->getTrainingLeft(capacityNumber)-1);
	if (this->getTrainingLeft(capacityNumber) == 0) {
		this->up(capacityNumber);
		this->setTrainingLeft(capacityNumber,this->getCapacity(capacityNumber)-1);
	}
}

gold ManagedPlayer::getEstimatedValue() {
	int index=0; //Index for value depends on the capacities and the number of trainings left for each capacity
	for (int i=0;i<5;++i){
		index+=2*this->getCapacity(i) - _trainingLeft[i];
	}

	return index*PRICESCALE + _broomstick.getValue(); //Value of player is the index * the price scale + the value of the broomstick
}

vector<int> ManagedPlayer::getInformations() {
	/*cout<<"------------------ "<<this->getFirstName()<<" "<<this->getLastName()<<" ------------------"<<endl;
	cout<<"Capacities :"<<endl;
	cout<<"Speed : "<<this->getCapacity(0)<<endl;
	cout<<"Strength : "<<this->getCapacity(1)<<endl;
	cout<<"Precision : "<<this->getCapacity(2)<<endl;
	cout<<"Reflex : "<<this->getCapacity(3)<<endl;
	cout<<"Resistance : "<<this->getCapacity(4)<<endl;
	cout<<"\nLife : "<<this->getLife()<<endl;
	cout<<"\nIs the player blocked ? "<<_blocked<<endl;
	cout<<"\nBroomstick bonus is "<<_broomstick.getBonus()<<" for capacity "<<_broomstick.getCapacityBoosted()<<endl;*/

	vector<int> tmp;
	for (int i=0;i<5;++i) tmp.push_back(this->getCapacity(i));
	for (int i=0;i<5;++i) tmp.push_back(_trainingLeft[i]);
	tmp.push_back(_blocked);
	tmp.push_back(_broomstick.getCapacityBoosted());
	tmp.push_back(_broomstick.getBonus());
	tmp.push_back(this->getLife());
	tmp.push_back(getEstimatedValue());

	return tmp;
}


//~ void ManagedPlayer::serialize(char * stringData){
//~ }
