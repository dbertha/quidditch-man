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

	int _numberOfPlayers;
	int _money;
	int _numberOfFans;
	int _actionPoints;

 	Stadium _stadium;
	TrainingCenter _trainingCenter;
	Hospital _hospital;
	FanShop _fanShop;
	PromotionCenter _promotionCenter;

 public:

 	Manager();
 	Manager(string managerLogin);

 	string getLogin();

 	int getNumberOfPlayers();
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
 	void removePlayer(ManagedPlayer& player);
 	vector<string> getPlayersList();
 	vector<ManagedPlayer> getPlayers();
 	ManagedPlayer getPlayer(int index);
 	vector<int> getPlayerInformations(int playerID);

 	Stadium& getStadium();
 	vector<int> getStadiumInformations();
 	gold getIncomeFromMatch(bool hasWon,bool wasHost);

 	TrainingCenter& getTrainingCenter();
 	vector<int> getTrainingCenterInformations();
 	bool trainPlayer(int playerID, int capacityNumber);
 	void unlockPlayer(string name);
 	bool isPlayerBlocked(int playerID);
 	bool isPlayerBlocked(string name);

 	Hospital& getHospital();
 	vector<int> getHospitalInformations();
 	bool healPlayer(int playerID);

 	FanShop& getFanShop();
 	vector<int> getFanShopInformations();
 	gold getIncomeFromFanShop();

 	PromotionCenter& getPromotionCenter();
 	vector<int> getPromotionCenterInformations();
 	void getGainFromPromotion();

 	int startStadiumConstruction();
 	int startTrainingCenterConstruction();
	int startHospitalConstruction();
	int startFanShopConstruction();
	int startPromotionCenterConstruction();

	void upgradeBuilding(string buildingName);

 	void writeBlockInCalendar(string name,bool isTraining);
	void writeInCalendar(string file, string name, int timeRequired);

};

#endif //Manager_hpp
