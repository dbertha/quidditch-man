
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
#include "DataBase.hpp"
#include "Building.hpp"
#include "ImprovementBuilding.hpp"
#include "Stadium.hpp"
#include "TrainingCenter.hpp"
#include "Hospital.hpp"
#include "FanShop.hpp"
#include "PromotionCenter.hpp"
#include "../common/Defines.hpp"

#include "Manager.hpp"


typedef int gold;

using namespace std;

Manager::Manager():_login(""),_numberOfPlayers(0),_money(0),_numberOfFans(0),_actionPoints(0) {}

Manager::Manager(string managerLogin): _login(managerLogin) {
	DataBase::load(*this);
}

////////////////////////////// METHODS ON ATTRIBUTES //////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
string Manager::getLogin() {return _login;}

int Manager::getNumberOfPlayers() {return _numberOfPlayers;}
void Manager::setNumberOfPlayers(int number) {_numberOfPlayers=number;}

int Manager::getMoney() {return _money;}
void Manager::setMoney(int number) {_money=number;}
void Manager::addMoney(int amount) {_money+=amount;}
void Manager::pay(int amount) {_money-=amount;}

int Manager::getNumberOfFans() {return _numberOfFans;}
void Manager::setNumberOfFans(int number) {_numberOfFans=number;}

int Manager::getActionPoints() {return _actionPoints;}
void Manager::setActionPoints(int number) {_actionPoints=number;}
int Manager::payForActionPoints(int numberOfAP){
	gold amount = numberOfAP * GOLD_FOR_ONE_AP;
	if (_money<amount) return 0;
	_money-=amount;
	_actionPoints+=numberOfAP;
	return amount;
}
int Manager::waitForActionPoints(int timeElapsed){
	int gain = _promotionCenter.getActionPointsGain();
	int APGained = gain * (timeElapsed/TIMESCALEACTIONPOINTS);
	_actionPoints += APGained;
	return APGained;
}

/////////////////////////////// METHODS ON PLAYERS ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void Manager::addPlayer(ManagedPlayer& player) {
	_players.push_back(player);
	++ _numberOfPlayers;
	DataBase::save(*this);
}
void Manager::setPlayer(ManagedPlayer* player) {
	ManagedPlayer tmp = *player;
	_players.push_back(tmp);
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
		string file = "server/Saves/"+_login+"/Players/"+playerName+".txt";
		remove(file.c_str());
		DataBase::save(*this);
	}
	else {
		throw "Player not found";
	}
}
vector<string> Manager::getPlayersList() {
	vector<string> tmp;
	for (int i=0;i<_numberOfPlayers;++i) {
		tmp.push_back(_players[i].getFirstName());
		tmp.push_back(_players[i].getLastName());
	}
	return tmp;
}
vector<ManagedPlayer> Manager::getPlayers() {return _players;}
ManagedPlayer Manager::getPlayer(int index) {
	if (index>=_numberOfPlayers) throw "Index out of range";
	return _players[index];
}
vector<int> Manager::getPlayerInformations(int playerID) {
	if (playerID>=_numberOfPlayers) throw "Index out of range";
	return _players[playerID].getInformations();
}

////////////////////////////// METHODS ON BUILDINGS ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
Stadium& Manager::getStadium() {return _stadium;}
vector<int> Manager::getStadiumInformations() {return _stadium.getInformations();}
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

TrainingCenter& Manager::getTrainingCenter() {return _trainingCenter;}
vector<int> Manager::getTrainingCenterInformations() {return _trainingCenter.getInformations();}
bool Manager::trainPlayer(int playerID, int capacityNumber) {
	if (isPlayerBlocked(playerID)) return false;
	if (_actionPoints<AP_TRAINING) return false;
	if (playerID>=_numberOfPlayers) throw "Index of player index out of range";
	if (capacityNumber>=5) throw "Capacity index out of range";
	_trainingCenter.train(_players[playerID],capacityNumber);
	string name = _players[playerID].getFirstName() + " " + _players[playerID].getLastName();
	writeBlockInCalendar(name,true);
	_actionPoints-=AP_TRAINING;
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

Hospital& Manager::getHospital() {return _hospital;}
vector<int> Manager::getHospitalInformations() {return _hospital.getInformations();}
bool Manager::healPlayer(int playerID) {
	if (isPlayerBlocked(playerID)) return false;
	if (_actionPoints<AP_HOSPITAL) return false;
	if (_players[playerID].getLife()==_players[playerID].getCapacity(4)) return false;
	if (playerID>=_numberOfPlayers) throw "Index out of range";
	_hospital.heal(_players[playerID]);
	string name = _players[playerID].getFirstName() + " " + _players[playerID].getLastName();
	writeBlockInCalendar(name,false);
	_actionPoints-=AP_HOSPITAL;
	return true;
}

FanShop& Manager::getFanShop() {return _fanShop;}
vector<int> Manager::getFanShopInformations() {return _fanShop.getInformations();}
gold Manager::getIncomeFromFanShop() {return _fanShop.getIncome();}

PromotionCenter& Manager::getPromotionCenter() {return _promotionCenter;}
vector<int> Manager::getPromotionCenterInformations() {return _promotionCenter.getInformations();}

int Manager::startStadiumConstruction() {
	if (_money<_stadium.getPriceForNextLevel()) return NOTENOUGHMONEY;
	if (_stadium.isUpgrading()) return ALREADYINCONSTRUCTION;
	if (_actionPoints<AP_UPGRADE) return NOTENOUGHAP;
	pay(_stadium.getPriceForNextLevel());
	_stadium.startConstruction();
	string file = "server/Saves/"+_login+"/constructionCalendar.txt";
	writeInCalendar(file,"Stadium", TIMESCALECONSTRUCTION*(1+_stadium.getLevel()));
	_actionPoints-=AP_UPGRADE;
	return CONSTRUCTIONSTARTED;
}
int Manager::startTrainingCenterConstruction() {
	if (_money<_trainingCenter.getPriceForNextLevel()) return NOTENOUGHMONEY;
	if (_trainingCenter.isUpgrading()) return ALREADYINCONSTRUCTION;
	if (_actionPoints<AP_UPGRADE) return NOTENOUGHAP;
	pay(_trainingCenter.getPriceForNextLevel());
	_trainingCenter.startConstruction();
	string file = "server/Saves/"+_login+"/constructionCalendar.txt";
	writeInCalendar(file,"TrainingCenter", TIMESCALECONSTRUCTION*(1+_trainingCenter.getLevel()));
	_actionPoints-=AP_UPGRADE;
	return CONSTRUCTIONSTARTED;
}
int Manager::startHospitalConstruction() {
	if (_money<_hospital.getPriceForNextLevel()) return NOTENOUGHMONEY;
	if (_hospital.isUpgrading()) return ALREADYINCONSTRUCTION;
	if (_actionPoints<AP_UPGRADE) return NOTENOUGHAP;
	pay(_hospital.getPriceForNextLevel());
	_hospital.startConstruction();
	string file = "server/Saves/"+_login+"/constructionCalendar.txt";
	writeInCalendar(file,"Hospital", TIMESCALECONSTRUCTION*(1+_hospital.getLevel()));
	_actionPoints-=AP_UPGRADE;
	return CONSTRUCTIONSTARTED;
}
int Manager::startFanShopConstruction() {
	if (_money<_fanShop.getPriceForNextLevel()) return NOTENOUGHMONEY;
	if (_fanShop.isUpgrading()) return ALREADYINCONSTRUCTION;
	if (_actionPoints<AP_UPGRADE) return NOTENOUGHAP;
	pay(_fanShop.getPriceForNextLevel());
	_fanShop.startConstruction();
	string file = "server/Saves/"+_login+"/constructionCalendar.txt";
	writeInCalendar(file,"FanShop", TIMESCALECONSTRUCTION*(1+_fanShop.getLevel()));
	_actionPoints-=AP_UPGRADE;
	return CONSTRUCTIONSTARTED;
}
int Manager::startPromotionCenterConstruction() {
	if (_money<_promotionCenter.getPriceForNextLevel()) return NOTENOUGHMONEY;
	if (_promotionCenter.isUpgrading()) return ALREADYINCONSTRUCTION;
	if (_actionPoints<AP_UPGRADE) return NOTENOUGHAP;
	pay(_promotionCenter.getPriceForNextLevel());
	_promotionCenter.startConstruction();
	string file = "server/Saves/"+_login+"/constructionCalendar.txt";
	writeInCalendar(file,"PromotionCenter", TIMESCALECONSTRUCTION*(1+_promotionCenter.getLevel()));
	_actionPoints-=AP_UPGRADE;
	return CONSTRUCTIONSTARTED;
}

void Manager::upgradeBuilding(string buildingName) {
	if (buildingName=="Stadium") _stadium.upgrade();
	else if (buildingName=="TrainingCenter") _trainingCenter.upgrade();
	else if (buildingName=="Hospital") _hospital.upgrade();
	else if (buildingName=="FanShop") _fanShop.upgrade();
	else if (buildingName=="PromotionCenter") _promotionCenter.upgrade();
}


////////////////////////////// METHODS ON CALENDARS ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void Manager::writeBlockInCalendar(string name,bool isTraining) {
	string file = "server/Saves/"+_login+"/blockCalendar.txt";
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

	//Get string for date. Format = day:month:hour:minute
	string date = DataBase::intToString(instant.tm_mday)+":"+DataBase::intToString(instant.tm_mon+1)+":"+\
	DataBase::intToString(instant.tm_hour)+":"+DataBase::intToString(instant.tm_min);

	string timeBlock = DataBase::intToString(timeRequired);

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

