
#include <string> 
#include <vector>
#include <sys/stat.h>
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

#include "ManagedPlayer.hpp"
#include "Saver.hpp"
//#include "Broomstick.hpp"

#include "Manager.hpp"


using namespace std;

Manager::Manager(string managerLogin) {
	string directory = "Saves/"+managerLogin;
	int result = mkdir(directory.c_str(),0777);
	if (result==-1) loadManager(managerLogin); //Directory already exists.
	else createNewManager(managerLogin);
}

void Manager::loadManager(string managerLogin) {
	string saveFile = "Saves/"+managerLogin+"/"+managerLogin+".txt";
	int fd = open("Saves/Manager/Manager.txt",O_RDONLY);
	if (fd==-1) {
		cerr<<"Error while opening file\n";
		return;
	}

	char buffer[100];
	int byte = read(fd,buffer,sizeof(buffer));
	buffer[byte]='\0';

	string tmp;
	tmp = strtok(buffer,"\n");
	_numberOfPlayers = atoi(tmp.c_str());

	//tmp = strtok(NULL,"\n");
	//_numberMaxOfPlayers = atoi(tmp.c_str());
	//_players = new ManagedPlayer[_numberMaxOfPlayers];


	tmp = strtok(NULL,"\n");
	_money = atoi(tmp.c_str());

	tmp = strtok(NULL,"\n");
	_numberOfFans = atoi(tmp.c_str());

	close(fd);
	

	string playerList = "Saves/"+managerLogin+"/Players/players.txt";
	int fd2 = open(playerList.c_str(),O_RDONLY);
	if (fd2==-1) {
		cerr<<"Error while opening file\n";
		return;
	}

	char buffer2[400];

	int byte2 = read(fd,buffer2,400);
	buffer2[byte2]='\0';

	
	string path = "Saves/"+managerLogin+"/Players/";
	string playerFile= "";

	int i=0;
	while (buffer2[i]!='\0'){
		if (buffer2[i]=='\n') {
			_players.push_back(ManagedPlayer(path+playerFile));			
			playerFile="";
		}
		else playerFile+=buffer2[i];
		++i;
	}
	
	close(fd2);

}
void Manager::createNewManager(string managerLogin) {
	_numberOfPlayers = 7;
	//_numberMaxOfPlayers = 7;
	_money = 500000;
	_numberOfFans = 666;

	_players.push_back(ManagedPlayer("Saves/defaultKeeper.txt"));
	_players.push_back(ManagedPlayer("Saves/defaultSeeker.txt"));
	for (int i=0;i<2;++i) _players.push_back(ManagedPlayer("Saves/defaultBeater.txt"));
	for (int i=0;i<3;++i) _players.push_back(ManagedPlayer("Saves/defaultChaser.txt"));

	Saver saver;
	saver.saveManager(managerLogin,*this);

	string directory = "Saves/"+managerLogin+"/Players";
	mkdir(directory.c_str(),0777);
	saver.savePlayersList(managerLogin,_players);
	for (unsigned i=0;i<_players.size();++i) saver.savePlayer(managerLogin,_players[i]);
	

}


int Manager::getNumberOfPlayers() {return _numberOfPlayers;}
//int Manager::getNumberMaxOfPlayers() {return _numberMaxOfPlayers;}
int Manager::getMoney() {return _money;}
int Manager::getNumberOfFans() {return _numberOfFans;}


void Manager::setNumberOfPlayers(int number) {_numberOfPlayers=number;}
void Manager::addMoney(int amount) {_money+=amount;}
void Manager::pay(int amount) {_money-=amount;}
void Manager::setNumberOfFans(int number) {_numberOfFans=number;}

ManagedPlayer Manager::getPlayer(int index) {
	if (index>=_numberOfFans) throw "Index out of range";
	return _players[index];
}

