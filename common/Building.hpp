#ifndef Building_hpp
#define Building_hpp

class Building {
 public: //a mettre priver
    int _level;
    int _priceForNextLevel;
    
 public:

	Building(int lvl=0);
//	Building();
	void upgradeLevel();
//    virtual void getInfos()  = 0;
	int getPriceForNextLevel();

};

#endif // Building_hpp
