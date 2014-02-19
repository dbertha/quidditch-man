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
 	void updateCalendar(string file, bool isBlockCalendar);
 	bool checkTime(char* line, bool isBlockCalendar);
 	bool compareToCurrentDate(int day, int month, int hour, int minute); //true if current date is later in time, false otherwise

};


#endif //Calendar_hpp
