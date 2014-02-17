#ifndef Building_hpp
#define Building_hpp

typedef int gold;

class Building {

 protected:
    int _level;
	const gold _priceForConstruction;
    
 public:

	Building(int lvl=0, gold price=100000);
//	Building();
	virtual void upgradeBuilding();
	virtual int getLevel();
	//virtual void getInfos() = 0;
	virtual gold getPriceForNextLevel() = 0;
};


#endif // Building_hpp

