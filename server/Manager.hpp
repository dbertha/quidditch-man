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
#include "PromotionCenter.hpp"

typedef int gold; //gold will be used instead of int everytime an int represents an amount of money

class Manager {

	string _login;
	vector<ManagedPlayer> _players;
	vector<Building*> _buildings;

	int _numberOfPlayers;
	int _money;
	int _numberOfFans;
	int _actionPoints;


 public:

 	Manager();
 	Manager(string managerLogin);

 	string getLogin();

 	int getNumberOfPlayers();
 	int getNumberOfNonBlockedPlayers();
 	void setNumberOfPlayers(int number);

 	int getMoney();
 	void setMoney(int number);
 	void addMoney(int amount);
 	void pay(int amount);

 	int getNumberOfFans();
 	void setNumberOfFans(int number);

 	int getActionPoints();
 	void setActionPoints(int number);
 	int payForActionPoints(int numberOfAP);
 	int waitForActionPoints(int timeElapsed);

 	void addPlayer(ManagedPlayer& player);
 	void setPlayer(ManagedPlayer* player);
 	void sellPlayer(int playerID);
 	void removePlayer(ManagedPlayer& player);
 	vector<string> getPlayersList();
 	vector<ManagedPlayer> getPlayers();
 	ManagedPlayer& getPlayer(int index);
 	vector<int> getPlayerInformations(int playerID);

 	vector<Building*>& getBuildings();
 	vector<int> getBuildingInformations(int buildingID);

 	gold getIncomeFromMatch(bool hasWon,bool wasHost);

 	bool trainPlayer(int playerID, int capacityNumber);
 	void lockPlayer(string name);
 	void unlockPlayer(string name);
 	bool isPlayerBlocked(int playerID);
 	bool isPlayerBlocked(string name);

 	bool healPlayer(int playerID);

 	gold getIncomeFromFanShop();

 	void getGainFromPromotion();

 	int startBuildingConstruction(int buildingID);

	void upgradeBuilding(int buildingID);

 	void writeBlockInCalendar(string name,bool isTraining);
	void writeInCalendar(string file, string name, int timeRequired);

};

#endif //Manager_hpp
