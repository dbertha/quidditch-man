
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
#include "Building.hpp"
#include "ImprovementBuilding.hpp"
#include "Stadium.hpp"
#include "TrainingCenter.hpp"
#include "Hospital.hpp"
#include "FanShop.hpp"
#include "RecruitmentCenter.hpp"

#include "Manager.hpp"

#define TICKETPRICE 40
#define VICTORYBONUS 35000

typedef int gold;

using namespace std;

Manager::Manager(string managerLogin): _login(managerLogin) {
	string directory = "Saves/"+managerLogin;
	int result = mkdir(directory.c_str(),0777);
	if (result==-1) loadManager(); //Directory already exists.
	else createNewManager();
}

void Manager::loadManager() {
	setManagerInfos("Saves/"+_login+"/"+_login+".txt");
	cout<<"setManagerInfos done"<<endl;
	setPlayers("Saves/"+_login+"/Players/");
	cout<<"setPlayers done"<<endl;
	setBuildings("Saves/"+_login+"/buildings.txt");
	cout<<"setBuildings done"<<endl;
}

void Manager::createNewManager() {
	setManagerInfos("Saves/defaultManager.txt");
	setPlayers("Saves/");
	setBuildings("Saves/defaultBuildings.txt");

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

gold Manager::getIncomeFromMatch(bool hasWon,bool wasHost) {
	gold money=0;
	if (wasHost) {
		if (_numberOfFans<_stadium.getMaxPlaces()) money+=_numberOfFans*TICKETPRICE;
		else money+=_stadium.getMaxPlaces()*TICKETPRICE;
	}
	if (hasWon) money+=VICTORYBONUS;
	return money;
}
void Manager::trainPlayer(int playerID, int capacityNumber) {_trainingCenter.train(_players[playerID],capacityNumber);}
void Manager::healPlayer(int playerID) {_hospital.heal(_players[playerID]);}
gold Manager::getIncomeFromFanShop() {return _fanShop.getIncome();}

void Manager::save(){
	Saver saver;
	saver.saveManager(_login,*this);
	for (unsigned i=0;i<_players.size();++i) saver.savePlayer(_login,_players[i]);
	saver.savePlayersList(_login,_players);
	saver.saveBuildings(_login,_stadium,_trainingCenter,_hospital,_fanShop,_recruitmentCenter);
}

void Manager::setManagerInfos(string file) {
	int fd = open(file.c_str(),O_RDONLY);
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

	tmp = strtok(NULL,"\n");
	_money = atoi(tmp.c_str());

	tmp = strtok(NULL,"\n");
	_numberOfFans = atoi(tmp.c_str());

	close(fd);
}

void Manager::setPlayers(string path) {
	string file = path + "players.txt";
	int fd = open(file.c_str(),O_RDONLY);
	if (fd==-1) {
		cerr<<"Error while opening file\n";
		return;
	}
	int size = lseek(fd,0,SEEK_END);
	lseek(fd,0,SEEK_SET);
	char buffer[size+1];

	int byte = read(fd,buffer,size);
	buffer[byte]='\0';

	string playerFile;
	int i=0;
	while (buffer[i]!='\0'){
		if (buffer[i]=='\n') {
			_players.push_back(ManagedPlayer(path+playerFile));			
			playerFile="";
		}
		else playerFile+=buffer[i];
		++i;
	}
	
	close(fd);
}

void Manager::setBuildings(string file) {
	int fd = open(file.c_str(),O_RDONLY);
	if (fd==-1) {
		cerr<<"Error while opening file\n";
		return;
	}
	int size = lseek(fd,0,SEEK_END);
	lseek(fd,0,SEEK_SET);
	char buffer[size+1];

	int byte = read(fd,buffer,size);
	buffer[byte]='\0';

	string tmp1,tmp2,tmp3;
	int param1,param2,param3;
	tmp1 = strtok(buffer,"\n");
	param1 = atoi(tmp1.c_str());
	tmp2 = strtok(NULL,"\n");
	param2 = atoi(tmp2.c_str());
	tmp3 = strtok(NULL,"\n");
	param3 = atoi(tmp3.c_str());
	_stadium = Stadium(param1,param2,param3);

	tmp1 = strtok(NULL,"\n");
	param1 = atoi(tmp1.c_str());
	tmp2 = strtok(NULL,"\n");
	param2 = atoi(tmp2.c_str());
	tmp3 = strtok(NULL,"\n");
	param3 = atoi(tmp3.c_str());
	_trainingCenter = TrainingCenter(param1,param2,param3);

	tmp1 = strtok(NULL,"\n");
	param1 = atoi(tmp1.c_str());
	tmp2 = strtok(NULL,"\n");
	param2 = atoi(tmp2.c_str());
	tmp3 = strtok(NULL,"\n");
	param3 = atoi(tmp3.c_str());
	_hospital = Hospital(param1,param2,param3);

	tmp1 = strtok(NULL,"\n");
	param1 = atoi(tmp1.c_str());
	tmp2 = strtok(NULL,"\n");
	param2 = atoi(tmp2.c_str());
	tmp3 = strtok(NULL,"\n");
	param3 = atoi(tmp3.c_str());
	_fanShop = FanShop(param1,param2,param3);

	tmp1 = strtok(NULL,"\n");
	param1 = atoi(tmp1.c_str());
	tmp2 = strtok(NULL,"\n");
	param2 = atoi(tmp2.c_str());
	_recruitmentCenter = RecruitmentCenter(param1,param2);

	close(fd);
}
