#ifndef Stadium_hpp
#define Stadium_hpp

#include "Building.hpp"


typedef int gold;

class Stadium : public Building {

	int _maxPlacesAtFirstLevel;

 public:

 	Stadium(int level=0, gold priceForConstruction=0, int maxPlaces=0);

    gold getPriceForNextLevel();
    int getMaxPlaces();
    int getMaxPlacesAtFirstLevel();
    void displayInformations();
    //void getInfos();


};

#endif //Stadium_hpp
