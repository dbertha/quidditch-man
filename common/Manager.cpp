
#include <string> 
#include <vector>
#include <sys/stat.h>
#include <iostream>
#include <stdlib.h>     /* atoi */
#include <ctime>
#include <cstdlib> 
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "ManagedPlayer.hpp"
#include "Saver.hpp"
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
#define TIMESCALECONSTRUCTION 1

typedef int gold;

using namespace std;

Manager::Manager(string managerLogin): _login(managerLogin) {
	string directory = "Saves/"+managerLogin;
	int result = mkdir(directory.c_str(),0777);
	if (result==-1) loadManager(); //Directory already exists so need to load a manager
	else createNewManager(); //else, manager need to be created
}

////////////////////////////// METHODS ON ATTRIBUTES //////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
string Manager::getLogin() {return _login;}

int Manager::getNumberOfPlayers() {return _numberOfPlayers;}
void Manager::setNumberOfPlayers(int number) {_numberOfPlayers=number;}

int Manager::getMoney() {return _money;}
void Manager::addMoney(int amount) {_money+=amount;}
void Manager::pay(int amount) {_money-=amount;}

int Manager::getNumberOfFans() {return _numberOfFans;}
void Manager::setNumberOfFans(int number) {_numberOfFans=number;}

/////////////////////////////// METHODS ON PLAYERS ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
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
	if (index>=_numberOfPlayers) throw "Index out of range";
	return _players[index];
}
void Manager::displayPlayerInformations(int playerID) {
	if (playerID>=_numberOfPlayers) throw "Index out of range";
	_players[playerID].displayInformations();
}

////////////////////////////// METHODS ON BUILDINGS ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void Manager::displayStadiumInformations() {_stadium.displayInformations();}
gold Manager::getIncomeFromMatch(bool hasWon,bool wasHost) {
	gold money=0;
	if (wasHost) { //Income from ticket sale. If manager has more fans than places in the stadium
		//Stadium will always be full
		if (_numberOfFans<_stadium.getMaxPlaces()) money+=_numberOfFans*TICKETPRICE;
		else money+=_stadium.getMaxPlaces()*TICKETPRICE;
	}
	if (hasWon) money+=VICTORYBONUS;
	return money;
}

void Manager::displayTrainingCenterInformations() {_trainingCenter.displayInformations();}
bool Manager::trainPlayer(int playerID, int capacityNumber) {
	if (isPlayerBlocked(playerID)) return false;
	if (playerID>=_numberOfPlayers) throw "IPlayer index out of range";
	if (capacityNumber>=5) throw "Capacity index out of range";
	_trainingCenter.train(_players[playerID],capacityNumber);
	string name = _players[playerID].getFirstName() + " " + _players[playerID].getLastName();
	writeBlockInCalendar(name,true);
	return true;
}
void Manager::unlockPlayer(string name) {
	string tmp;
	for (unsigned i=0;i<_players.size();++i){
		tmp = _players[i].getFirstName() + " " + _players[i].getLastName();
		if (tmp==name) {
			_players[i].unlockPlayer();
		}
	}
}
bool Manager::isPlayerBlocked(int playerID) {
	if (playerID>=_numberOfPlayers) throw "Index out of range";
	return _players[playerID].isBlocked();
}
bool Manager::isPlayerBlocked(string name) {
	string tmp;
	for (unsigned i=0;i<_players.size();++i){
		tmp = _players[i].getFirstName() + " " + _players[i].getLastName();
		if (tmp==name) {
			return _players[i].isBlocked();
		}
	}
	return false;
}

void Manager::displayHospitalInformations() {_hospital.displayInformations();}
bool Manager::healPlayer(int playerID) {
	if (isPlayerBlocked(playerID)) return false;
	if (playerID>=_numberOfPlayers) throw "Index out of range";
	_hospital.heal(_players[playerID]);
	string name = _players[playerID].getFirstName() + " " + _players[playerID].getLastName();
	writeBlockInCalendar(name,false);
	return true;
}

void Manager::displayFanShopInformations() {_fanShop.displayInformations();}
gold Manager::getIncomeFromFanShop() {return _fanShop.getIncome();}

void Manager::displayRecruitmentCenterInformations() {_recruitmentCenter.displayInformations();}

bool Manager::startStadiumConstruction() {
	if (_money<_stadium.getPriceForNextLevel()) return false;
	pay(_stadium.getPriceForNextLevel());
	string file = "Saves/"+_login+"/constructionCalendar.txt";
	writeInCalendar(file,"Stadium", TIMESCALECONSTRUCTION*(7-_stadium.getLevel()));
	return true;
}
bool Manager::startTrainingCenterConstruction() {
	if (_money<_trainingCenter.getPriceForNextLevel()) return false;
	pay(_trainingCenter.getPriceForNextLevel());
	string file = "Saves/"+_login+"/constructionCalendar.txt";
	writeInCalendar(file,"TrainingCenter", TIMESCALECONSTRUCTION*(7-_trainingCenter.getLevel()));
	return true;
}
bool Manager::startHospitalConstruction() {
	if (_money<_hospital.getPriceForNextLevel()) return false;
	pay(_hospital.getPriceForNextLevel());
	string file = "Saves/"+_login+"/constructionCalendar.txt";
	writeInCalendar(file,"Hospital", TIMESCALECONSTRUCTION*(7-_hospital.getLevel()));
	return true;
}
bool Manager::startFanShopConstruction() {
	if (_money<_fanShop.getPriceForNextLevel()) return false;
	pay(_fanShop.getPriceForNextLevel());
	string file = "Saves/"+_login+"/constructionCalendar.txt";
	writeInCalendar(file,"FanShop", TIMESCALECONSTRUCTION*(7-_fanShop.getLevel()));
	return true;
}
bool Manager::startRecruitmentCenterConstruction() {
	if (_money<_recruitmentCenter.getPriceForNextLevel()) return false;
	pay(_recruitmentCenter.getPriceForNextLevel());
	string file = "Saves/"+_login+"/constructionCalendar.txt";
	writeInCalendar(file,"RecruitmentCenter", TIMESCALECONSTRUCTION*(7-_recruitmentCenter.getLevel()));
	return true;
}

void Manager::upgradeBuilding(string buildingName) {
	if (buildingName=="Stadium") _stadium.upgrade();
	else if (buildingName=="TrainingCenter") _trainingCenter.upgrade();
	else if (buildingName=="Hospital") _hospital.upgrade();
	else if (buildingName=="FanShop") _fanShop.upgrade();
	else if (buildingName=="RecruitmentCenter") _recruitmentCenter.upgrade();
}


////////////////////////////// METHODS ON CALENDARS ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void Manager::writeBlockInCalendar(string name,bool isTraining) {
	string file = "Saves/"+_login+"/blockCalendar.txt";
	int timeRequired; //in minutes
	if (isTraining) timeRequired = _trainingCenter.getTimeRequired();
	else timeRequired = _hospital.getTimeRequired();

	writeInCalendar(file,name,timeRequired);


}
void Manager::writeInCalendar(string file, string name, int timeRequired) {
	time_t secondes;
	struct tm instant;
	time(&secondes);
	instant=*localtime(&secondes);

	Saver saver; //pour intToString

	//Get string for date. Format = day:month:hour:minute
	string date = saver.intToString(instant.tm_mday)+":"+saver.intToString(instant.tm_mon+1)+":"+\
	saver.intToString(instant.tm_hour)+":"+saver.intToString(instant.tm_min);

	string timeBlock = saver.intToString(timeRequired);

	int fd = open(file.c_str(),O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd==-1){
		cerr<<"Error while opening file\n";
		return;
	}

	//Now, we write the line for the calendar. Format = PlayerOrBuilding#StartingDate#timeRequiredInMinutes#
	write(fd,name.c_str(),name.size());
	write(fd,"#",1);
	write(fd,date.c_str(),date.size());
	write(fd,"#",1);
	write(fd,timeBlock.c_str(),timeBlock.size());
	write(fd,"#",1);
	write(fd,"\n",1);

	close(fd);
}


//////////////////////////// METHOD TO SAVE THE MANAGER ///////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void Manager::save(){
	Saver saver;
	saver.saveManager(_login,*this);
	for (unsigned i=0;i<_players.size();++i) saver.savePlayer(_login,_players[i]);
	saver.savePlayersList(_login,_players);
	saver.saveBuildings(_login,_stadium,_trainingCenter,_hospital,_fanShop,_recruitmentCenter);
}


////////////////////////////// LOAD/CREATE A MANAGER //////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void Manager::loadManager() {
	setManagerInfos("Saves/"+_login+"/"+_login+".txt");
	setPlayers("Saves/"+_login+"/Players/");
	setBuildings("Saves/"+_login+"/buildings.txt");
}

void Manager::createNewManager() {
	//"Create" a new manager is loading a default Manager whom files are in Saves/
	setManagerInfos("Saves/defaultManager.txt");
	setPlayers("Saves/"); //SetPlayers just need a path, not a precise file. Since we're not supposed to know how many players there are
	setBuildings("Saves/defaultBuildings.txt");

	//The directory for the Manager, which is "Saves/ManagerLogin" has already been created in the constructor
	string directory = "Saves/"+_login+"/Players"; //So we just need to create the repertory Players in the manager repertory
	mkdir(directory.c_str(),0777);

	//We create the files for the calendars (currently empty)
	string calendar = "Saves/"+_login+"/constructionCalendar.txt";
	int fd = open(calendar.c_str(),O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
	close(fd);
	calendar = "Saves/"+_login+"/blockCalendar.txt";
	fd = open(calendar.c_str(),O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
	close(fd);
	//And by calling the save method, the informations of the new manager (buildings, players, etc) will be saved in the manager repertory
	save();
}


///////////////////////// GET INFORMATIONS FROM SAVEFILES /////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void Manager::setManagerInfos(string file) {
	/*
	This method reads the manager informations in the Manager.txt file..
	This file format is :
		NumberOfPlayers
		Money
		NumberOfFans
	*/
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
	/*
	This method reads all the players informations. Each player has its own save file "FirstNameLastName.txt".
	Since we don't know the players names, the name of each player save file is written in the "players.txt" file
	In this file, each line is the name of a player save file. So by reading players.txt, we're able to get the informations
	 of every player.
*/
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

	string playerFile; // player save file; "FirstNameLastName.txt"
	int i=0;
	while (buffer[i]!='\0'){
		if (buffer[i]=='\n') {
			_players.push_back(ManagedPlayer(path+playerFile));	//ManagedPlayer constructor will read the needed informations in the file		
			playerFile="";
		}
		else playerFile+=buffer[i];
		++i;
	}
	
	close(fd);
}

void Manager::setBuildings(string file) {
	/*
	This method reads the buildings informations in the buildings.txt file..
	This file format is :
		Level of the stadium
		Price to build the stadium (construction is the upgrade from level 0 to level 1)
		Max places in the stadium at level 1
		Level of the training center
		Price to build the training center
		Time required to finish a training session at level 1
		Level of the hospital
		Price to build the hospital
		Time required to heal a player at level 1
		Level of the fan shop
		Price to build the fan shop
		Max clients in the fan shop at level 1
		Level of the recruitment center
		Price to build the recruitment center
	The price to upgrade a building is calculated by the building itself and depends on the price of construction and the current level
	The special attribute of the stadium, the training center, the hospital and the fan shop is calculated by the building and depends
	 on the current level
	*/


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
