
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

Player::Player(string playerSaveFile) {
	/*
	The format of a player save file is :
		First Name
		Last Name
		Speed 
		Strength 
		Precision
		Reflex 
		Resistance
		Life
		Training left to do to up speed (used by the constructor of ManagedPlayer)
		Training left to do to up strength (used by the constructor of ManagedPlayer)
		Training left to do to up precision (used by the constructor of ManagedPlayer)
		Training left to do to up reflex (used by the constructor of ManagedPlayer)
		Training left to do to up resistance (used by the constructor of ManagedPlayer)
		1 if player is blocked (by the training center or the hospital) or 0 if not (used by the constructor of ManagedPlayer)
		Number of the capacity boosted by the broomstick (used by the constructor of ManagedPlayer)
		Bonus granted by the broomstick (used by the constructor of ManagedPlayer)

	The constructor needs to read the first part of the file, the rest will be read by the constructor of ManagedPlayer.
	The attribute _offset will indicate to ManagedPlayer where to start reading.
	*/
	int fd = open(playerSaveFile.c_str(),O_RDONLY);
	if (fd==-1) {
		perror("Fuck1");
		cerr<<"Error while opening file\n";
		return;
	}
	
	char buffer[100];
	int bytes;
	bytes = read(fd,buffer,sizeof(buffer));
	buffer[bytes]='\0';

	_firstName = strtok(buffer,"\n");
	_lastName = strtok(NULL,"\n");
	_offset = _firstName.size()+1 + _lastName.size()+1; //Size of the first name, the last name, and +1 for each "\n"

	string tmp;
	for (int i=0;i<5;++i){
		tmp = strtok(NULL,"\n");
		_offset+= tmp.size()+1; //Size of the line, +1 for the "\n"
		_capacities[i] = atoi(tmp.c_str());
	}

	tmp = strtok(NULL,"\n");
	_offset+= tmp.size()+1;
	_life = atoi(tmp.c_str());

	close(fd);

	//default name is John Doe. If it's the player name, it's a new player, and he needs a real name.
	if ((_firstName=="John")&&(_lastName=="Doe")) verifyName(); 
}

Player& Player::operator= (const Player& player){
	for (int i=0;i<5;++i) _capacities[i] = player.getCapacity(i);
	return *this;
}

string Player::getFirstName() const {return _firstName;}
string Player::getLastName() const {return _lastName;}

void Player::verifyName() {
	//verifyName will go get a random first name and a random last name in the files firstNames.txt and lastNames.txt
	//While the combination of the two already exists (there is already a player who's called like that), the name is present in namesTaken.txt
	//Once a name who doesn't already exist is found, we can name the player, and add this name in namesTaken.txt
	srand(time(0));
	do {
		int firstNameIndex, lastNameIndex;
		firstNameIndex = rand()%100 +1;
		lastNameIndex = rand()%100 +1;

		_firstName = getRandomFirstName(firstNameIndex);
		_lastName = getRandomLastName(lastNameIndex);

	} while (isNameTaken()); //check if name already exists
	
	int fd = open("server/Saves/namesTaken.txt",O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd==-1){
		cerr<<"Error while opening file\n";
	}
	//Add the new name in namesTaken.txt
	write(fd,_firstName.c_str(),_firstName.size());
	write(fd," ",1);
	write(fd,_lastName.c_str(),_lastName.size());
	write(fd,"\n",1);
	close(fd);
	
}
bool Player::isNameTaken(){
	//Check if a line in namesTaken.txt is the same name as the current one
	int fd = open("server/Saves/namesTaken.txt",O_RDONLY);
	if (fd==-1){
		cerr<<"Error while opening file\n";
		return 0;
	}
	int size = lseek(fd, 0, SEEK_END); 
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
			return 1;
		}
		first="";
		last="";
	}

	close(fd);
	return 0;
}

string Player::getRandomName(string fileName,int line){
		char buffer[12*line]; //Longest name is 12 characters long. So a buffer of size 12*numberOfline will always contain the 12th line

		int fd;
		fd = open(fileName.c_str(),O_RDONLY);
		if (fd==-1) {
			cerr<<"Error while opening file\n";
			return "Error";
		}
		int byte = read(fd,buffer,sizeof(buffer));
		buffer[byte]='\0';

		int i = 1; //Line number
		int j = 0;
		while ((i!=line)&&(j<byte)){
			if (buffer[j]=='\n') ++i; //If character read is "\n", it's a new line.
			++j;
		}
		string name = "";
		while ((j<byte)&&(buffer[j]!='\n')) { //The name is every character in the buffer until a "\n" or the end of the buffer is reached
			name+=buffer[j];
			++j;
		}
		close(fd);
		return name;

}

int Player::getCapacity(int capacityNumber) const {return _capacities[capacityNumber];}
void Player::setCapacity(int capacityNumber, int value) {_capacities[capacityNumber] = value;}

int Player::getLife() const {return _life;}
void Player::setLife(int value) {_life = value;}
void Player::heal() {_life = _capacities[4];}

string Player::getRandomFirstName(int line) {return getRandomName("server/Saves/firstNames.txt",line);}
string Player::getRandomLastName(int line) { return getRandomName("server/Saves/lastNames.txt",line);}

void Player::setFirstName(string firstName) {_firstName = firstName;}
void Player::setLastName(string lastName) {_lastName = lastName;}
void Player::up(int capacityNumber) {_capacities[capacityNumber]+=1;}
