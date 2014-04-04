#ifndef Building_hpp
#define Building_hpp

#include <vector>

#include "ManagedPlayer.hpp"
typedef int gold;

class Building {

 protected:
    int _level;
	gold _priceForConstruction;
	bool _isUpgrading;
    
 public:

	Building(int lvl, gold price, bool isUpgrading);
	virtual void upgrade();
	virtual int getLevel();
	virtual gold getPriceForConstruction();
	virtual bool isUpgrading();
	virtual void startConstruction();
	/*/
	virtual gold getIncome();
	virtual int getTimeRequired();
	virtual void heal(ManagedPlayer&);
	virtual void train(ManagedPlayer&, int);
	virtual int getMaxPlaces();
	virtual int getMaxClients();
	virtual int getActionPointsGain();
	virtual int getMaxPlacesAtFirstLevel();
	virtual int getGainAtFirstLevel();
	virtual int getMaxClientsAtFirstLevel();
	virtual int getTimeRequiredAtFirstLevel();
	/*/
	virtual std::vector<int> getInformations() =0;
	virtual gold getPriceForNextLevel() = 0; //calcul is not the same for every building
};


#endif // Building_hpp

