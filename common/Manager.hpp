#ifndef Manager_hpp
#define Manager_hpp

#include <string> 

#include "Broomstick.hpp"
//#include "Building.hpp"
//#include "Calendar.hpp"
//#include "ClientHandler.hpp"
#include "ManagedPlayer.hpp"

class Manager {

	ManagedPlayer* _players;
	//Building[] _buildings;
	int _numberOfPlayers;
	int _numberMaxOfPlayers;
	int _money;
	int _numberOfFans;

 public:

 	Manager(string managerLogin);

 	int getNumberOfPlayers();
 	int getNumberMaxOfPlayers();
 	int getMoney();
 	int getNumberOfFans();

 	ManagedPlayer getPlayer(int index);

 private:

 	void loadManager(string managerLogin);
 	void createNewManager(string managerLogin);

};

#endif //Manager_hpp
