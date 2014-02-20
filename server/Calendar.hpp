#ifndef Calendar_hpp
#define Calendar_hpp

#include <stdio.h>
#include <time.h>
#include <string>

#include "Manager.hpp"

class Calendar {

	Manager* _manager;

 public:
 	Calendar();
 	Calendar(Manager* manager);
 	void update(); //wil call updateCalendar for constructionCalender and blockCalendar
 	void updateCalendar(string file, bool isBlockCalendar); //will call checkTime for every line in the calendar "file"
 	bool checkTime(char* line, bool isBlockCalendar); //verify a line in a calender (block is bool parameter is true, construction otherwise)
 	bool compareToCurrentDate(int day, int month, int hour, int minute); //true if current date is later in time, false otherwise

};


#endif //Calendar_hpp
