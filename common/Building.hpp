#ifndef Building_hpp
#define Building_hpp

class Building {
 public: //a mettre priver
    int _level;
    int _priceForNextLevel;
    
 public:

	Building(int lvl=0);
//	Building();
	virtual void upgradeLevel();
	virtual int getLevel();
//    virtual void getInfos()  = 0;
	virtual int getPriceForNextLevel();
	virtual void setPriceForNextlevel();
};

#endif // Building_hpp
