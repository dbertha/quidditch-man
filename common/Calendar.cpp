

#include <string> 
#include <vector>
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

Calendar::Calendar(Manager& manager): _manager(manager) {}

void Calendar::update() {
	string file = "Saves/"+_manager.getLogin()+"/constructionCalendar.txt";
	updateCalendar(file,false);
	file = "Saves/"+_manager.getLogin()+"/blockCalendar.txt";
	updateCalendar(file,true);
}

void Calendar::updateCalendar(string file, bool isBlockCalendar) {
	int fd = open(file.c_str(),O_RDONLY);
	if (fd==-1){
		cerr<<"Error while opening file"<<endl;
		return;
	}

	string file_tmp = "Saves/"+_manager.getLogin()+"/tmp.txt";
	int fd_tmp = open(file_tmp.c_str(),O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd_tmp==-1){
		cerr<<"Error while opening file"<<endl;
		return;
	}

	bool change=false;

	int size = lseek(fd,0,SEEK_END);
	lseek(fd,0,SEEK_SET);
	char buffer[size+1];

	int byte = read(fd,buffer,size);
	buffer[byte]='\0';

	string name, date, blockTime;

	char* line = strtok(buffer,"\n");
	if (line!=NULL){ 
		string toWrite = line;
		if (checkTime(line,isBlockCalendar)) change=true;
		while ((line=strtok(NULL,"\n"))!=NULL) {
			if (!checkTime(line,isBlockCalendar)){
				write(fd_tmp,toWrite.c_str(),toWrite.size());
				write(fd_tmp,"\n",1);
			}
			else change=true;
		}
	}
	close(fd);
	close(fd_tmp);
	if (change){
		remove(file.c_str());
		rename(file_tmp.c_str(),file.c_str());
	}
	else {
		remove(file_tmp.c_str());
	}

}

bool Calendar::checkTime(char* line, bool isBlockCalendar) {
	string name;
	char* date;
	string blockTime;
	name = strtok(line,"#");
	date = strtok(NULL,"#");
	blockTime = strtok(NULL,"#");
	if ( (isBlockCalendar&&(_manager.isPlayerBlocked(name))) || (!isBlockCalendar) ) {
		string sMonth, sDay, sHour, sMinute;
		int month, day, hour, minute;
		sDay = strtok(date,":");
		sMonth = strtok(NULL,":");
		sHour = strtok(NULL,":");
		sMinute = strtok(NULL,":");
		month = atoi(sMonth.c_str());
		day = atoi(sDay.c_str());
		hour = atoi(sHour.c_str());
		minute = atoi(sMinute.c_str());

		int block = atoi(blockTime.c_str());
		for (int i=1;i<block+1;++i){
			minute+=1;
			if (minute==60) {
				hour+=1;
				minute=0;
				if (hour==24){
					day+=1;
					hour=0;
					if ( ((day==32)&&((month==1)||(month==3)||(month==5)||(month==7)||(month==8)||(month==10)||(month==12))) || \
					( (day==31)&&((month==4)||(month==6)||(month==9)||(month==11)) ) || ( (day==29)&&(month=2) ) ) {
						month+=1;
						if (month==13) month=1;
						day=1;
					} 
				}
			}
		}

	    if (compareToCurrentDate(day,month,hour,minute)) {
	    	if (isBlockCalendar) _manager.unlockPlayer(name);
	    	else _manager.upgradeBuilding(name);
	    	return true;
	    }
	}
	return false;

}

bool Calendar::compareToCurrentDate(int day, int month, int hour, int minute) {
		time_t secondes;
	    struct tm instant;
	    time(&secondes);
	    instant=*localtime(&secondes);
	    bool hasTimePassed = false;
	    if (month==instant.tm_mon+1){
	    	if (day==instant.tm_mday){
	    		if (hour==instant.tm_hour){
	    			if (minute<=instant.tm_min) hasTimePassed=true;
	    		}
	    		else if (hour<instant.tm_hour) hasTimePassed=true;
	    	}
	    	else if (day<instant.tm_mday) hasTimePassed=true;
	    }
	    else if (month<instant.tm_mon+1) hasTimePassed=true;

	   return hasTimePassed;
}

/*
int main(void)
{
    time_t secondes;
    struct tm instant;
    time(&secondes);
    instant=*localtime(&secondes);
    printf("%d/%d ; %d:%d:%d\n", instant.tm_mday+1, instant.tm_mon+1, instant.tm_hour, instant.tm_min, instant.tm_sec);
    return 0;
}
*/