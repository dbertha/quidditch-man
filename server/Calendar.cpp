#include <string> 
#include <sys/stat.h>
#include <iostream>
#include <stdlib.h>     /* atoi */
#include <ctime>
#include <cstdlib> 
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "Manager.hpp"
#include "Calendar.hpp"

using namespace std;

Calendar::Calendar() : _manager(NULL) {}

Calendar::Calendar(Manager* manager): _manager(manager) {}

void Calendar::update() {
	string file = "server/Saves/"+_manager->getLogin()+"/constructionCalendar.txt";
	updateCalendar(file,false);
	file = "server/Saves/"+_manager->getLogin()+"/blockCalendar.txt";
	updateCalendar(file,true);
}

void Calendar::updateCalendar(string file, bool isBlockCalendar) {
	int fd = open(file.c_str(),O_RDONLY);
	if (fd==-1){
		cerr<<"Error while opening file"<<endl;
		return;
	}

	string file_tmp = "server/Saves/"+_manager->getLogin()+"/tmp.txt";
	//file_tmp will be used to change the calendar if lines must be erased
	int fd_tmp = open(file_tmp.c_str(),O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd_tmp==-1){
		cerr<<"Error while opening file"<<endl;
		return;
	}

	bool change=false; //will be true if at least one line must be erased

	int size = lseek(fd,0,SEEK_END);
	lseek(fd,0,SEEK_SET);
	char buffer[size+1];

	int byte = read(fd,buffer,size);
	buffer[byte]='\0';

	char* line, *context1;
	string toWrite;
	if ((line=strtok_r(buffer,"\n",&context1))!=NULL){
		toWrite = line;
		if (checkTime(line,isBlockCalendar)) change=true; //if checkTime is true, construction/block is finished
		else {
			write(fd_tmp,toWrite.c_str(),toWrite.size());
			write(fd_tmp,"\n",1);
		}
		while ((line=strtok_r(NULL,"\n",&context1))!=NULL) {
			toWrite = line;
			if (!checkTime(line,isBlockCalendar)){
				//if construction/block not finished, line cannot be erased.
				//so line is written in new file
				write(fd_tmp,toWrite.c_str(),toWrite.size());
				write(fd_tmp,"\n",1);
			}
			else change=true;
		}
	}
	close(fd);
	close(fd_tmp);
	if (change){ //if new calendar (file_tmp) != old calendar (file)
		remove(file.c_str()); //old calendar is deleted
		rename(file_tmp.c_str(),file.c_str()); //new calendar is renamed after old calendar
	}
	else { //new calendar == old calendar
		remove(file_tmp.c_str()); //new calendar is deleted
	}

}

bool Calendar::checkTime(char* line, bool isBlockCalendar) {
	string name;
	char* date, *context2, *context3;
	string blockTime;
	name = strtok_r(line,"#",&context2);
	date = strtok_r(NULL,"#",&context2);
	blockTime = strtok_r(NULL,"#",&context2);
	if ( (isBlockCalendar&&(_manager->isPlayerBlocked(name))) || (!isBlockCalendar) ) {
		//if blockCalendar, but player concerned "is not blocked", the manager does not have this player anymore
		//so there's no need to check if the player can be unlocked
		//if constructionCalendar, we always have to check

		string sMonth, sDay, sHour, sMinute;
		int month, day, hour, minute;
		sDay = strtok_r(date,":",&context3);
		sMonth = strtok_r(NULL,":",&context3);
		sHour = strtok_r(NULL,":",&context3);
		sMinute = strtok_r(NULL,":",&context3);
		month = atoi(sMonth.c_str());
		day = atoi(sDay.c_str());
		hour = atoi(sHour.c_str());
		minute = atoi(sMinute.c_str());

		int block = atoi(blockTime.c_str());

		//timeRequired as read in calendar is in minutes, so we need to update the date read in file
		//This date represents the moment when the construction/block started
		for (int i=1;i<block+1;++i){
			minute+=1;
			if (minute==60) { //an hour has passed
				hour+=1; 
				minute=0;
				if (hour==24){ //a day has passed
					day+=1;
					hour=0;
					if ( ((day==32)&&((month==1)||(month==3)||(month==5)||(month==7)||(month==8)||(month==10)||(month==12))) || \
					( (day==31)&&((month==4)||(month==6)||(month==9)||(month==11)) ) || ( (day==29)&&(month=2) ) ) {
						//a month has passed (the changement of month depends on the correlation between the day and the month)
						month+=1;
						if (month==13) month=1; //No need to check or update year. This game won't last that long.
						day=1;
					} 
				}
			}
		}

	    if (compareToCurrentDate(day,month,hour,minute)) { //is the date of the end of construction/block is later or earlier than 'now' ?
	    	//if it's earlier, construction/block is finished, so changement need to be made for the manager
	    	if (isBlockCalendar) _manager->unlockPlayer(name); //if block, player has to be unlocked
	    	else _manager->upgradeBuilding(atoi(name.c_str())); //if construction, building has to be upgraded
	    	return true; //line is no longer required in the calendar file
	    }
	}
	return false; //line is still required in calendar

}

bool Calendar::compareToCurrentDate(int day, int month, int hour, int minute) {
	//current date :
	time_t secondes;
	struct tm instant;
	time(&secondes);
	instant=*localtime(&secondes);
	//
	bool hasTimePassed = false;
	if (month==instant.tm_mon+1){ //if same month
	    if (day==instant.tm_mday){ //and same day
			if (hour==instant.tm_hour){ //and same hour
    			if (minute<=instant.tm_min) hasTimePassed=true; //and same minute or before; time has passed
    		}
    		else if (hour<instant.tm_hour) hasTimePassed=true; //if hour of end is before current hour; time has passed
    	}
    	else if (day<instant.tm_mday) hasTimePassed=true; //if day of end is before current day; time has passed
    }
    else if (month<instant.tm_mon+1) hasTimePassed=true; //if month of end is before current month; time has passed

    //else, time hasn't passed yet.

   return hasTimePassed;
}

int Calendar::getElapsedTime(struct tm startTime) {
	int elapsedTime;
	int minutesOfStart, minutesNow;

	minutesOfStart=startTime.tm_min + 60 * startTime.tm_hour + 24 * 60 * startTime.tm_mday;

	time_t secondes;
	struct tm instant;
	time(&secondes);
	instant=*localtime(&secondes);

	minutesNow=instant.tm_min + 60 * instant.tm_hour + 24 * 60 * instant.tm_mday;

	elapsedTime = minutesNow-minutesOfStart;

	std::cout<<"MINUTES NOW : "<<minutesNow<<std::endl;
	std::cout<<"MINUTES OF START : "<<minutesOfStart<<std::endl;
	std::cout<<"ELAPSED TIME : "<<elapsedTime<<std::endl;

	return elapsedTime;
}
