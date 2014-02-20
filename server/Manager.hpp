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

typedef int gold; //gold will be used instead of int everytime an int represents an amount of money

class Manager {

	string _login;
	vector<ManagedPlayer> _players;

	int _numberOfPlayers;
	int _money;
	int _numberOfFans;

 	Stadium _stadium;
	TrainingCenter _trainingCenter;
	Hospital _hospital;
	FanShop _fanShop;
	RecruitmentCenter _recruitmentCenter;

 public:

 	Manager();
 	Manager(string managerLogin);

 	string getLogin();

 	int getNumberOfPlayers();
 	void setNumberOfPlayers(int number);

 	int getMoney();
 	void addMoney(int amount);
 	void pay(int amount);

 	int getNumberOfFans();
 	void setNumberOfFans(int number);

 	void addPlayer(ManagedPlayer& player);
 	void removePlayer(ManagedPlayer& player);
 	vector<string> getPlayersList();
 	ManagedPlayer getPlayer(int index);
 	vector<int> getPlayerInformations(int playerID);

 	vector<int> getStadiumInformations();
 	gold getIncomeFromMatch(bool hasWon,bool wasHost);

 	vector<int> getTrainingCenterInformations();
 	bool trainPlayer(int playerID, int capacityNumber);
 	void unlockPlayer(string name);
 	bool isPlayerBlocked(int playerID);
 	bool isPlayerBlocked(string name);

 	vector<int> getHospitalInformations();
 	bool healPlayer(int playerID);

 	vector<int> getFanShopInformations();
 	gold getIncomeFromFanShop();

 	vector<int> getRecruitmentCenterInformations();

 	bool startStadiumConstruction();
 	bool startTrainingCenterConstruction();
	bool startHospitalConstruction();
	bool startFanShopConstruction();
	bool startRecruitmentCenterConstruction();

	void upgradeBuilding(string buildingName);

 	void writeBlockInCalendar(string name,bool isTraining);
	void writeInCalendar(string file, string name, int timeRequired);

 	void save();

 private:

 	void loadManager();
 	void createNewManager();

 	void setManagerInfos(string file);
 	void setPlayers(string path);
 	void setBuildings(string file);

};

#endif //Manager_hpp
