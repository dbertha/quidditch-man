#ifndef Building_hpp
#define Building_hpp

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
	virtual gold getPriceForNextLevel() = 0; //calcul is not the same for every building
};


#endif // Building_hpp

