#ifndef Stadium_hpp
#define Stadium_hpp

#include "Building.hpp"
#include <vector>

typedef int gold;

class Stadium : public Building {

	int _maxPlacesAtFirstLevel;

 public:

 	Stadium(int level=0, gold priceForConstruction=0, int maxPlaces=0);

    gold getPriceForNextLevel();
    int getMaxPlaces();
    int getMaxPlacesAtFirstLevel();
    std::vector<int> getInformations();


};

#endif //Stadium_hpp
