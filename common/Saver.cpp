
#include "Manager.hpp"
#include "ManagedPlayer.hpp"
#include "Saver.hpp"


#include <string> 
#include <vector>
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

using namespace std;

void Saver::saveManager(string managerLogin, Manager manager) {
	string file = "Saves/"+managerLogin+"/"+managerLogin+".txt";
	int fd = open(file.c_str(),O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd==-1){
		cerr<<"Error while opening file\n";
	}

	string toWrite;

	toWrite = intToString(manager.getNumberOfPlayers());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);

	//toWrite = intToString(manager.getNumberMaxOfPlayers());
	//write(fd,toWrite.c_str(),toWrite.size());
	//write(fd,"\n",1);

	toWrite = intToString(manager.getMoney());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);

	toWrite = intToString(manager.getNumberOfFans());
	write(fd,toWrite.c_str(),toWrite.size());

	close(fd);
}

void Saver::savePlayer(string managerLogin, ManagedPlayer player) {
	string file = "Saves/"+managerLogin+"/Players/"+player.getFirstName()+player.getLastName()+".txt";
	int fd = open(file.c_str(),O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd==-1){
		cerr<<"Error while opening file\n";
	}

	string toWrite;

	toWrite = player.getFirstName();
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);

	toWrite = player.getLastName();
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);

	for (int i=0;i<5;++i) {
		toWrite = intToString(player.getCapacity(i));
		write(fd,toWrite.c_str(),toWrite.size());
		write(fd,"\n",1);
	}

	for (int i=0;i<5;++i) {
		toWrite = intToString(player.getTrainingLeft(i));
		write(fd,toWrite.c_str(),toWrite.size());
		write(fd,"\n",1);
	}
/*
	toWrite = intToString(player.getPopularity());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);
*/
	toWrite = intToString(player.isBlocked());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);

	Broomstick broomstick = player.getBroomstick();
	toWrite = intToString(broomstick.getCapacityBoosted());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);

	toWrite = intToString(broomstick.getBonus());
	write(fd,toWrite.c_str(),toWrite.size());


	close(fd);
}

void Saver::savePlayersList(string managerLogin, vector<ManagedPlayer> players) {
	string file = "Saves/"+managerLogin+"/Players/players.txt";
	remove(file.c_str());
	int fd = open(file.c_str(),O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd==-1){
		cerr<<"Error while opening file\n";
	}

	string toWrite;
	for (unsigned i=0;i<players.size();++i){
		toWrite=players[i].getFirstName()+players[i].getLastName()+".txt";
		write(fd,toWrite.c_str(),toWrite.size());
		write(fd,"\n",1);
	}
	close(fd);
}

string Saver::intToString(int value) {
	char buffer[20];
	sprintf(buffer,"%d",value);
	string tmp = "";
	int i = 0;
	while (buffer[i] != '\0') {
		tmp+=buffer[i];
		++i;
	}
	return tmp;
}