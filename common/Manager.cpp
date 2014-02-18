
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

Manager::Manager(string managerLogin): _login(managerLogin) {
	string directory = "Saves/"+managerLogin;
	int result = mkdir(directory.c_str(),0777);
	if (result==-1) loadManager(); //Directory already exists.
	else createNewManager();
}

void Manager::loadManager() {
	string saveFile = "Saves/"+_login+"/"+_login+".txt";
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
	

	string playerList = "Saves/"+_login+"/Players/players.txt";
	int fd2 = open(playerList.c_str(),O_RDONLY);
	if (fd2==-1) {
		cerr<<"Error while opening file\n";
		return;
	}

	char buffer2[400];

	int byte2 = read(fd,buffer2,400);
	buffer2[byte2]='\0';

	
	string path = "Saves/"+_login+"/Players/";
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

void Manager::createNewManager() {
	_numberOfPlayers = 7;
	//_numberMaxOfPlayers = 7;
	_money = 500000;
	_numberOfFans = 666;

	_players.push_back(ManagedPlayer("Saves/defaultKeeper.txt"));
	_players.push_back(ManagedPlayer("Saves/defaultSeeker.txt"));
	for (int i=0;i<2;++i) _players.push_back(ManagedPlayer("Saves/defaultBeater.txt"));
	for (int i=0;i<3;++i) _players.push_back(ManagedPlayer("Saves/defaultChaser.txt"));

	string directory = "Saves/"+_login+"/Players";
	mkdir(directory.c_str(),0777);

	save();
}

int Manager::getNumberOfPlayers() {return _numberOfPlayers;}
void Manager::setNumberOfPlayers(int number) {_numberOfPlayers=number;}
//int Manager::getNumberMaxOfPlayers() {return _numberMaxOfPlayers;}
int Manager::getMoney() {return _money;}
void Manager::addMoney(int amount) {_money+=amount;}
void Manager::pay(int amount) {_money-=amount;}

int Manager::getNumberOfFans() {return _numberOfFans;}
void Manager::setNumberOfFans(int number) {_numberOfFans=number;}

void Manager::addPlayer(ManagedPlayer& player) {
	_players.push_back(player);
	++ _numberOfPlayers;
	save();
}
void Manager::removePlayer(ManagedPlayer& player) {
	string playerName = player.getFirstName() + player.getLastName();
	string tmp;
	int index=-1;
	for (unsigned i=0;i<_players.size();++i){
		tmp = _players[i].getFirstName() + _players[i].getLastName();
		if (tmp==playerName) {
			index=i;
			i=_players.size();
		}
	}
	if (index!=-1){
		_players.erase(_players.begin()+index);

		-- _numberOfPlayers;
		string file = "Saves/"+_login+"/Players/"+playerName+".txt";
		remove(file.c_str());
		save();
	}
	else {
		throw "Player not found";
	}
}

ManagedPlayer Manager::getPlayer(int index) {
	if (index>=_numberOfFans) throw "Index out of range";
	return _players[index];
}

void Manager::save(){
	Saver saver;
	saver.saveManager(_login,*this);
	for (unsigned i=0;i<_players.size();++i) saver.savePlayer(_login,_players[i]);
	saver.savePlayersList(_login,_players);
}

