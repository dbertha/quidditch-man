#ifndef Manager_hpp
#define Manager_hpp

#include <string> 
#include <vector>


#include "ManagedPlayer.hpp"
#include "Broomstick.hpp"
#include "Building.hpp"
#include "ImprovementBuilding.hpp"
#include "Stadium.hpp"
#include "TrainingCenter.hpp"
#include "Hospital.hpp"
#include "FanShop.hpp"
#include "RecruitmentCenter.hpp"

typedef int gold;

class Manager {

	string _login;
	vector<ManagedPlayer> _players;

	int _numberOfPlayers;
	int _money;
	int _numberOfFans;

 public:
 	Stadium _stadium;
	TrainingCenter _trainingCenter;
	Hospital _hospital;
	FanShop _fanShop;
	RecruitmentCenter _recruitmentCenter;

 	Manager(string managerLogin);

 	int getNumberOfPlayers();
 	void setNumberOfPlayers(int number);
 	//int getNumberMaxOfPlayers();
 	int getMoney();
 	void addMoney(int amount);
 	void pay(int amount);

 	int getNumberOfFans();
 	void setNumberOfFans(int number);

 	void addPlayer(ManagedPlayer& player);
 	void removePlayer(ManagedPlayer& player);

 	ManagedPlayer getPlayer(int index);

 	gold getIncomeFromMatch(bool hasWon,bool wasHost);
 	void trainPlayer(int playerID, int capacityNumber);
 	void writeBlockInCalendar(string name,bool isTraining);
 	void unlockPlayer(string name);
 	bool isPlayerBlocked(int playerID);
 	bool isPlayerBlocked(string name);
 	void healPlayer(int playerID);
 	gold getIncomeFromFanShop();

 	void startStadiumConstruction();
 	void startTrainingCenterConstruction();
	void startHospitalConstruction();
	void startFanShopConstruction();
	void startRecruitmentCenterConstruction();

	void upgradeBuilding(string buildingName);

	void writeInCalendar(string file, string name, int timeRequired);

 	string getLogin();

 	void save();

 private:

 	void loadManager();
 	void createNewManager();

 	void setManagerInfos(string file);
 	void setPlayers(string path);
 	void setBuildings(string file);

};

#endif //Manager_hpp
