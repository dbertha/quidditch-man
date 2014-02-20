#ifndef Building_hpp
#define Building_hpp

typedef int gold;

class Building {

 protected:
    int _level;
	gold _priceForConstruction;
    
 public:

	Building(int lvl, gold price);
	virtual void upgrade();
	virtual int getLevel();
	virtual gold getPriceForConstruction();
	virtual gold getPriceForNextLevel() = 0; //calcul is not the same for every building
};


#endif // Building_hpp

