#ifndef Saver_hpp
#define Saver_hpp

#include <string>
#include <vector>

#include "Manager.hpp"
#include "ManagedPlayer.hpp"

class Saver {


 public:

 	void saveManager(string managerLogin, Manager manager);
 	void savePlayer(string managerLogin, ManagedPlayer player);
 	void savePlayersList(string managerLogin,vector<ManagedPlayer> players);
 	string intToString(int value);

};




#endif //Saver_hpp