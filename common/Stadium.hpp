#ifndef Stadium_hpp
#define Stadium_hpp

#include "Building.hpp"


typedef int gold;

class Stadium : public Building {

	const int _maxPlacesAtFirstLevel;

 public:

 	Stadium(int level, int priceForConstruction, int maxPlaces);

    gold getPriceForNextLevel();
    int getMaxPlaces();

    //void getInfos();


};

#endif //Stadium_hpp
