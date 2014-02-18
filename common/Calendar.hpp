#ifndef Calendar_hpp
#define Calendar_hpp

#include <stdio.h>
#include <time.h>
#include <string>

#include "Manager.hpp"

class Calendar {

	Manager& _manager;

 public:
 	Calendar(Manager& manager);
 	void update();
 	bool checkBlock(char* line);

};


#endif //Calendar_hpp
