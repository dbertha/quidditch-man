#ifndef Manager_hpp
#define Manager_hpp

#include <string> 
#include <vector>

#include "Broomstick.hpp"
//#include "Building.hpp"
//#include "Calendar.hpp"
//#include "ClientHandler.hpp"
#include "ManagedPlayer.hpp"

class Manager {

	std::string _login;
	vector<ManagedPlayer> _players;
	//Building[] _buildings;
	int _numberOfPlayers;
	//int _numberMaxOfPlayers;
	int _money;
	int _numberOfFans;

 public:

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

 	void save();

 private:

 	void loadManager();
 	void createNewManager();

};

#endif //Manager_hpp
