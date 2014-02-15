
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

Player::Player() : _offset(0) {
	for (int i=0;i<5;++i) _capacities[i]=0;
}

Player::Player(string playerSaveFile) {
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
	_offset = _firstName.size()+1 + _lastName.size()+1; //+1 pour chaque '\n'

	string tmp;
	for (int i=0;i<5;++i){
		tmp = strtok(NULL,"\n");
		_offset+= tmp.size()+1;
		_capacities[i] = atoi(tmp.c_str());
	}

	close(fd);

	if ((_firstName=="John")&&(_lastName=="Doe")) verifyName();
}

Player& Player::operator= (const Player& player){
	for (int i=0;i<5;++i) _capacities[i] = player.getCapacity(i);
	return *this;
}

string Player::getFirstName() const {return _firstName;}
string Player::getLastName() const {return _lastName;}

void Player::verifyName() {
	srand(time(0));
	do {
		int firstNameIndex, lastNameIndex;
		firstNameIndex = rand()%100 +1;
		lastNameIndex = rand()%100 +1;

		_firstName = getRandomFirstName(firstNameIndex);
		_lastName = getRandomLastName(lastNameIndex);

	} while (isNameTaken());
	
	int fd = open("namesTaken.txt",O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd==-1){
		cerr<<"Error while opening file\n";
	}

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

int Player::getCapacity(int capacityNumber) const {return _capacities[capacityNumber];}
void Player::setCapacity(int capacityNumber, int value) {_capacities[capacityNumber] = value;}

string Player::getRandomFirstName(int line) {return getRandomName("firstNames.txt",line);}
string Player::getRandomLastName(int line) { return getRandomName("lastNames.txt",line);}

void Player::setFirstName(string firstName) {_firstName = firstName;}
void Player::setLastName(string lastName) {_lastName = lastName;}
void Player::up(int capacityNumber) {_capacities[capacityNumber]+=1;}
