
#include "Player.hpp"
#include "Broomstick.hpp"

#include <fstream>
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

Player::Player(string playerSaveFile) {
	int fd = open(playerSaveFile.c_str(),O_RDONLY);
	if (fd==-1) {
		cerr<<"Error while opening file\n";
		return;
	}
	
	char first[200];
	int bytes;
	bytes = read(fd,first,sizeof(first));
	first[bytes]='\0';

	_firstName = strtok(first,"\n");
	_lastName = strtok(NULL,"\n");

	string tmp;
	for (int i=0;i<5;++i){
		tmp = strtok(NULL,"\n");
		_capacities[i] = atoi(tmp.c_str());
	}

	for (int i=0;i<5;++i){
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

		_firstName = getRandomName("firstNames.txt",firstNameIndex);
		_lastName = getRandomName("lastNames.txt",lastNameIndex);

	} while (isNameTaken());
	
	int fd = open("namesTaken.txt",O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd==-1){
		cerr<<"Error while opening file\n";
	}

	const char * firstName = _firstName.c_str();
	const char * lastName = _lastName.c_str();
	cout<<"test : "<<firstName<<" "<<lastName<<endl;

	write(fd,_firstName.c_str(),_firstName.size());
	write(fd," ",1);
	write(fd,_lastName.c_str(),_lastName.size());
	write(fd,"\n",1);
	close(fd);
	
}
bool Player::isNameTaken(){
	int fd = open("namesTaken.txt",O_RDONLY);
	if (fd==-1){
		cerr<<"Error while opening file\n";
		return 0;
	}
	int size = lseek(fd, 0, SEEK_END); //Taille du fichier, pour être sûr de tout parcourir
	lseek(fd, 0, SEEK_SET);

	char tmp[size];

	int byte = read(fd,tmp,size);

	int i = 0;
	string first,last;
	first = "";
	last = "";
	while (i<byte) {
		while ((i<byte)&&(tmp[i]!=' ')){
			first+=tmp[i];
			++i;
		}
		++i;
		while ((i<byte)&&(tmp[i]!='\n')){
			last+=tmp[i];
			++i;
		}
		++i;

		if ((first==_firstName)&&(last==_lastName)) {
			cout<<"Name taken : "<<first<<" "<<last<<endl;
			return 1;
		}
		first="";
		last="";
	}

	close(fd);
	return 0;
}

string Player::getRandomName(string fileName,int line){
		char buffer[12*line]; //Le plus long nom/prénom est de 12. On s'assure d'avoir la ligne voulue dans le buffer

		int fd;
		fd = open(fileName.c_str(),O_RDONLY);
		if (fd==-1) {
			cerr<<"Error while opening file\n";
			return "Error";
		}
		int byte = read(fd,buffer,sizeof(buffer));
		buffer[byte]='\0';

		int i = 1; //Indique la ligne
		int j = 0;
		while ((i!=line)&&(j<byte)){
			if (buffer[j]=='\n') ++i;
			++j;
		}
		string name = "";
		while ((j<byte)&&(buffer[j]!='\n')) {
			name+=buffer[j];
			++j;
		}
		close(fd);
		return name;

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