#ifndef Saver_hpp
#define Saver_hpp

#include <string>
#include <vector>

#include "Manager.hpp"
#include "ManagedPlayer.hpp"

class DataBase {


 public:

 	static void save(Manager manager);
 	static void load(Manager& manager);

 	static void saveManager(string file, Manager manager);
 	static void savePlayer(string file, ManagedPlayer player);
 	static void savePlayersList(string file,vector<ManagedPlayer> players);
 	static void saveBuildings(string file, Stadium stadium, TrainingCenter trainingCenter, Hospital hospital, FanShop fanShop,\
 							 PromotionCenter promotionCenter);

 	static void loadManager(string file, Manager& manager);
 	static ManagedPlayer* loadPlayer(string file);
 	static void loadBuildings(string file, Stadium& stadium, TrainingCenter& trainingCenter, Hospital& hospital, FanShop& fanShop,\
 							 PromotionCenter& promotionCenter);

 	static string intToString(int value);

	static void verifyName(string& firstName, string& lastName);
	static bool isNameTaken(string firstName, string lastName);
	static string getRandomName(string fileName,int line);
	static string getRandomFirstName(int line);
	static string getRandomLastName(int line);

};




#endif //Saver_hpp