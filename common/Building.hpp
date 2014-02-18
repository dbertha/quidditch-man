#ifndef Building_hpp
#define Building_hpp

typedef int gold;

class Building {

 protected:
    int _level;
	gold _priceForConstruction;
    
 public:

	Building(int lvl, gold price);
//	Building();
	virtual void upgradeBuilding();
	virtual int getLevel();
	virtual gold getPriceForConstruction();
	//virtual void getInfos() = 0;
	virtual gold getPriceForNextLevel() = 0;
};


#endif // Building_hpp

