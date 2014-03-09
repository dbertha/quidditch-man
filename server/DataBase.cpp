
#include "Manager.hpp"
#include "ManagedPlayer.hpp"
#include "DataBase.hpp"


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

void DataBase::save(Manager manager) {
	string login = manager.getLogin();
	string file;

	file = "server/Saves/"+login+"/"+login+".txt";
	saveManager(file,manager);
	cout<<"saveManager done"<<endl;

	string directory = "server/Saves/"+login+"/Players";
	mkdir(directory.c_str(),0777);

	for (int i=0;i<manager.getNumberOfPlayers();++i) {
		ManagedPlayer player = manager.getPlayer(i);
		cout<<player.getFirstName()<<endl;
		file = "server/Saves/"+login+"/Players/"+player.getFirstName()+player.getLastName()+".txt";
		savePlayer(file,player);
	}

	cout<<"savePlayers done"<<endl;
	file = "server/Saves/"+login+"/Players/players.txt";
	savePlayersList(file,manager.getPlayers());

	cout<<"savePlayersList done"<<endl;
	file = "server/Saves/"+login+"/buildings.txt";
	saveBuildings(file,manager.getStadium(),manager.getTrainingCenter(),manager.getHospital(),manager.getFanShop(),manager.getPromotionCenter());

}

void DataBase::load(Manager& manager) {
	cout<<"load"<<endl;
	string login = manager.getLogin();
	string directory = "server/Saves/"+login;
	int result = mkdir(directory.c_str(),0777);
	
	string file;
	if (result==-1) file = "server/Saves/"+login+"/"+login+".txt";
	else file = "server/Saves/defaultManager.txt";
	loadManager(file,manager);

	string path;
	if (result==-1) path = "server/Saves/"+login+"/Players/";
	else path = "server/Saves/";

	string playersList = path + "players.txt";
	int fd = open(playersList.c_str(),O_RDONLY);
	if (fd==-1) {
		cerr<<"Error while opening players.txt file\n";
		return;
	}
	int size = lseek(fd,0,SEEK_END);
	lseek(fd,0,SEEK_SET);
	char buffer[size+1];

	int byte = read(fd,buffer,size);
	buffer[byte]='\0';

	string playerFile; // player save file; "FirstNameLastName.txt"
	int i=0;
	while (buffer[i]!='\0'){
		if (buffer[i]=='\n') {
			ManagedPlayer* player = loadPlayer(path+playerFile);
			manager.setPlayer(player);
			playerFile="";
		}
		else playerFile+=buffer[i];
		++i;
	}

	close(fd);

	if (result==-1) file = "server/Saves/"+login+"/buildings.txt";
	else file = "server/Saves/defaultBuildings.txt";
	loadBuildings(file,manager.getStadium(),manager.getTrainingCenter(),manager.getHospital(),manager.getFanShop(),manager.getPromotionCenter());
}

void DataBase::saveManager(string file, Manager manager) {
	//string file = "server/Saves/"+managerLogin+"/"+managerLogin+".txt";
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
	write(fd,"\n",1);

	toWrite = intToString(manager.getActionPoints());
	write(fd,toWrite.c_str(),toWrite.size());

	close(fd);
}

void DataBase::savePlayer(string file, ManagedPlayer player) {
	//string file = "server/Saves/"+managerLogin+"/Players/"+player.getFirstName()+player.getLastName()+".txt";
	int fd = open(file.c_str(),O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd==-1){
		cerr<<"Error while opening file\n";
	}

	cout<<"savePlayer"<<endl;
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
	toWrite = intToString(player.getLife());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);
	
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

void DataBase::savePlayersList(string file, vector<ManagedPlayer> players) {
	//string file = "server/Saves/"+managerLogin+"/Players/players.txt";
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

void DataBase::saveBuildings(string file, Stadium stadium, TrainingCenter trainingCenter, Hospital hospital, FanShop fanShop,\
 							 PromotionCenter promotionCenter) {
	//string file = "server/Saves/"+managerLogin+"/buildings.txt";
	int fd = open(file.c_str(),O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd==-1){
		cerr<<"Error while opening file\n";
	}
	string toWrite;
	toWrite=intToString(stadium.getLevel());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);
	toWrite=intToString(stadium.getPriceForConstruction());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);
	toWrite=intToString(stadium.getMaxPlacesAtFirstLevel());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);
	toWrite=intToString(stadium.isUpgrading());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);

	toWrite=intToString(trainingCenter.getLevel());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);
	toWrite=intToString(trainingCenter.getPriceForConstruction());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);
	toWrite=intToString(trainingCenter.getTimeRequiredAtFirstLevel());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);
	toWrite=intToString(trainingCenter.isUpgrading());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);

	toWrite=intToString(hospital.getLevel());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);
	toWrite=intToString(hospital.getPriceForConstruction());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);
	toWrite=intToString(hospital.getTimeRequiredAtFirstLevel());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);
	toWrite=intToString(hospital.isUpgrading());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);

	toWrite=intToString(fanShop.getLevel());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);
	toWrite=intToString(fanShop.getPriceForConstruction());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);
	toWrite=intToString(fanShop.getMaxClientsAtFirstLevel());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);
	toWrite=intToString(fanShop.isUpgrading());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);

	toWrite=intToString(promotionCenter.getLevel());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);
	toWrite=intToString(promotionCenter.getPriceForConstruction());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);
	toWrite=intToString(promotionCenter.getGainAtFirstLevel());
	write(fd,toWrite.c_str(),toWrite.size());
	write(fd,"\n",1);
	toWrite=intToString(promotionCenter.isUpgrading());
	write(fd,toWrite.c_str(),toWrite.size());
	
	close(fd);
}


void DataBase::loadManager(string file, Manager& manager){
	/*
	This method reads the manager informations in the Manager.txt file..
	This file format is :
		NumberOfPlayers
		Money
		NumberOfFans
		ActionPoints
	*/
	cout<<"loadManager"<<endl;
	int fd = open(file.c_str(),O_RDONLY);
	if (fd==-1) {
		cerr<<"Error while opening file\n";
		return;
	}

	char buffer[100];
	int byte = read(fd,buffer,sizeof(buffer));
	buffer[byte]='\0';

	string tmp;
	tmp = strtok(buffer,"\n");
	manager.setNumberOfPlayers(atoi(tmp.c_str()));

	tmp = strtok(NULL,"\n");
	manager.setMoney(atoi(tmp.c_str()));

	tmp = strtok(NULL,"\n");
	manager.setNumberOfFans(atoi(tmp.c_str()));

	tmp = strtok(NULL,"\n");
	manager.setActionPoints(atoi(tmp.c_str()));

	close(fd);

}

ManagedPlayer* DataBase::loadPlayer(string file){
	/*
	The format of a player save file is :
		First Name
		Last Name
		Speed 
		Strength 
		Precision
		Reflex 
		Resistance
		Life
		Training left to do to up speed
		Training left to do to up strength 
		Training left to do to up precision 
		Training left to do to up reflex 
		Training left to do to up resistance 
		1 if player is blocked (by the training center or the hospital) or 0 if not
		Number of the capacity boosted by the broomstick
		Bonus granted by the broomstick 

	*/

	cout<<"loadPlayer"<<endl;
	cout<<file<<endl;
	ManagedPlayer* player = new ManagedPlayer();

	int fd = open(file.c_str(),O_RDONLY);
	if (fd==-1) {
		cerr<<"Error while opening file\n";
	}

	int size = lseek(fd,0,SEEK_END);
	lseek(fd,0,SEEK_SET);
	
	char buffer[size+1];
	int bytes;
	bytes = read(fd,buffer,sizeof(buffer));
	buffer[bytes]='\0';

	string firstName,lastName;
	firstName = strtok(buffer,"\n");
	lastName = strtok(NULL,"\n");
	
	string tmp;
	for (int i=0;i<5;++i){
		tmp = strtok(NULL,"\n");
		player->setCapacity(i,atoi(tmp.c_str()));
	}

	tmp = strtok(NULL,"\n");
	player->setLife(atoi(tmp.c_str()));

	for (int i=0;i<5;++i){
		tmp = strtok(NULL,"\n");
		player->setTrainingLeft(i,atoi(tmp.c_str()));
	}

	tmp = strtok(NULL,"\n");
	int blocked = atoi(tmp.c_str());
	if (blocked) player->lockPlayer();
	else player->unlockPlayer();

	tmp = strtok(NULL,"\n");
	int broomstickCapacity = atoi(tmp.c_str());

	tmp = strtok(NULL,"\n");
	int broomstickBonus = atoi(tmp.c_str());

	player->setBroomstick(Broomstick(broomstickCapacity,broomstickBonus));

	close(fd);

	//default name is John Doe. If it's the player name, it's a new player, and he needs a real name.
	if ((firstName=="John")&&(lastName=="Doe")) verifyName(firstName,lastName); 

	player->setFirstName(firstName);
	player->setLastName(lastName);
	cout<<firstName<<" "<<lastName<<endl;
	return player;
}

void DataBase::loadBuildings(string file, Stadium& stadium, TrainingCenter& trainingCenter, Hospital& hospital, FanShop& fanShop,\
 							 PromotionCenter& promotionCenter){
	/*
	This method reads the buildings informations in the buildings.txt file..
	This file format is :
		Level of the stadium
		Price to build the stadium (construction is the upgrade from level 0 to level 1)
		Max places in the stadium at level 1
		Level of the training center
		Price to build the training center
		Time required to finish a training session at level 1
		Level of the hospital
		Price to build the hospital
		Time required to heal a player at level 1
		Level of the fan shop
		Price to build the fan shop
		Max clients in the fan shop at level 1
		Level of the promotion center
		Price to build the promotion center
		Amout of action points gained every 10 minutes at level 1
	The price to upgrade a building is calculated by the building itself and depends on the price of construction and the current level
	The special attribute of the stadium, the training center, the hospital and the fan shop is calculated by the building and depends
	 on the current level
	*/

	cout<<"loadBuilding"<<endl;

	int fd = open(file.c_str(),O_RDONLY);
	if (fd==-1) {
		cerr<<"Error while opening file\n";
		return;
	}
	int size = lseek(fd,0,SEEK_END);
	lseek(fd,0,SEEK_SET);
	char buffer[size+1];

	int byte = read(fd,buffer,size);
	buffer[byte]='\0';

	string tmp1,tmp2,tmp3,tmp4;
	int param1,param2,param3,param4;
	tmp1 = strtok(buffer,"\n");
	param1 = atoi(tmp1.c_str());
	tmp2 = strtok(NULL,"\n");
	param2 = atoi(tmp2.c_str());
	tmp3 = strtok(NULL,"\n");
	param3 = atoi(tmp3.c_str());
	tmp4 = strtok(NULL,"\n");
	param4 = atoi(tmp4.c_str());
	stadium = Stadium(param1,param2,param3,param4);

	tmp1 = strtok(NULL,"\n");
	param1 = atoi(tmp1.c_str());
	tmp2 = strtok(NULL,"\n");
	param2 = atoi(tmp2.c_str());
	tmp3 = strtok(NULL,"\n");
	param3 = atoi(tmp3.c_str());
	tmp4 = strtok(NULL,"\n");
	param4 = atoi(tmp4.c_str());
	trainingCenter = TrainingCenter(param1,param2,param3,param4);

	tmp1 = strtok(NULL,"\n");
	param1 = atoi(tmp1.c_str());
	tmp2 = strtok(NULL,"\n");
	param2 = atoi(tmp2.c_str());
	tmp3 = strtok(NULL,"\n");
	param3 = atoi(tmp3.c_str());
	tmp4 = strtok(NULL,"\n");
	param4 = atoi(tmp4.c_str());
	hospital = Hospital(param1,param2,param3,param4);

	tmp1 = strtok(NULL,"\n");
	param1 = atoi(tmp1.c_str());
	tmp2 = strtok(NULL,"\n");
	param2 = atoi(tmp2.c_str());
	tmp3 = strtok(NULL,"\n");
	param3 = atoi(tmp3.c_str());
	tmp4 = strtok(NULL,"\n");
	param4 = atoi(tmp4.c_str());
	fanShop = FanShop(param1,param2,param3,param4);

	tmp1 = strtok(NULL,"\n");
	param1 = atoi(tmp1.c_str());
	tmp2 = strtok(NULL,"\n");
	param2 = atoi(tmp2.c_str());
	tmp3 = strtok(NULL,"\n");
	param3 = atoi(tmp3.c_str());
	param4 = atoi(tmp4.c_str());
	promotionCenter = PromotionCenter(param1,param2,param3,param4);

	close(fd);

}

string DataBase::intToString(int value) {
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


void DataBase::verifyName(string& firstName, string& lastName) {
	//verifyName will go get a random first name and a random last name in the files firstNames.txt and lastNames.txt
	//While the combination of the two already exists (there is already a player who's called like that), the name is present in namesTaken.txt
	//Once a name who doesn't already exist is found, we can name the player, and add this name in namesTaken.txt
	
	srand(time(0));
	do {
		int firstNameIndex, lastNameIndex;
		firstNameIndex = rand()%100 +1;
		lastNameIndex = rand()%100 +1;

		firstName = getRandomFirstName(firstNameIndex);
		lastName = getRandomLastName(lastNameIndex);

	} while (isNameTaken(firstName,lastName)); //check if name already exists
	
	int fd = open("server/Saves/namesTaken.txt",O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd==-1){
		cerr<<"Error while opening file\n";
	}
	//Add the new name in namesTaken.txt
	write(fd,firstName.c_str(),firstName.size());
	write(fd," ",1);
	write(fd,lastName.c_str(),lastName.size());
	write(fd,"\n",1);
	close(fd);
	
}
bool DataBase::isNameTaken(string firstName, string lastName){
	//Check if a line in namesTaken.txt is the same name as the current one
	int fd = open("server/Saves/namesTaken.txt",O_RDONLY);
	if (fd==-1){
		cerr<<"Error while opening file\n";
		return 0;
	}
	int size = lseek(fd, 0, SEEK_END); 
	lseek(fd, 0, SEEK_SET);

	char tmp[size];

	int byte = read(fd,tmp,size);

	int i = 0;
	string first,last;
	first = "";
	last = "";
	while (i<byte) {
		while ((i<byte)&&(tmp[i]!=' ')){
			first+=tmp[i];
			++i;
		}
		++i;
		while ((i<byte)&&(tmp[i]!='\n')){
			last+=tmp[i];
			++i;
		}
		++i;

		if ((first==firstName)&&(last==lastName)) {
			return 1;
		}
		first="";
		last="";
	}

	close(fd);
	return 0;
}

string DataBase::getRandomName(string fileName,int line){
		char buffer[12*line]; //Longest name is 12 characters long. So a buffer of size 12*numberOfline will always contain the 12th line

		int fd;
		fd = open(fileName.c_str(),O_RDONLY);
		if (fd==-1) {
			cerr<<"Error while opening file\n";
			return "Error";
		}
		int byte = read(fd,buffer,sizeof(buffer));
		buffer[byte]='\0';

		int i = 1; //Line number
		int j = 0;
		while ((i!=line)&&(j<byte)){
			if (buffer[j]=='\n') ++i; //If character read is "\n", it's a new line.
			++j;
		}
		string name = "";
		while ((j<byte)&&(buffer[j]!='\n')) { //The name is every character in the buffer until a "\n" or the end of the buffer is reached
			name+=buffer[j];
			++j;
		}
		close(fd);
		return name;

}


string DataBase::getRandomFirstName(int line) {return getRandomName("server/Saves/firstNames.txt",line);}
string DataBase::getRandomLastName(int line) {return getRandomName("server/Saves/lastNames.txt",line);}