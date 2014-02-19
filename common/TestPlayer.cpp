
#include "Player.hpp"
#include "ManagedPlayer.hpp"
#include "PlayingPlayer.hpp"
#include "Broomstick.hpp"
#include "Saver.hpp"
#include "Building.hpp"
#include "ImprovementBuilding.hpp"
#include "Stadium.hpp"
#include "TrainingCenter.hpp"
#include "Hospital.hpp"
#include "FanShop.hpp"
#include "RecruitmentCenter.hpp"
#include "Calendar.hpp"

#include "Manager.hpp"

#include <iostream>

using namespace std;


int main() {
/*
	Manager manager("Manager");

	manager.displayStadiumInformations();
	manager.displayTrainingCenterInformations();
	manager.displayHospitalInformations();
	manager.displayFanShopInformations();
	manager.displayRecruitmentCenterInformations();

	//manager.startStadiumConstruction();

	//manager.getPlayer(1).displayInformations();
	//manager.trainPlayer(1,1);

	Calendar calendar(manager);

	calendar.update();
	//manager.getPlayer(1).displayInformations();
	manager.save();

/*
	Manager manager2("Manager2");

	cout<<manager.getIncomeFromMatch(true,true)<<endl;
	cout<<manager.getIncomeFromFanShop()<<endl;
*/

	ManagedPlayer player("Saves/defaultSeeker.txt");
	cout<<player.getFirstName()<<" "<<player.getLastName()<<endl;
	for (int i=0;i<5;++i){
		cout<<player.getCapacity(i)<<endl;
	}
	//cout<<"Popularity : "<<player.getPopularity()<<endl;
	cout<<"Is blocked ? "<<player.isBlocked()<<endl;

	cout<<"Training capacity"<<endl;
	player.train(1);
	cout<<"1st training, capacity = "<<player.getCapacity(1)<<endl;
	player.train(1);
	cout<<"2nd training, capacity = "<<player.getCapacity(1)<<endl;

	player.train(1);
	cout<<"3rd training, capacity = "<<player.getCapacity(1)<<endl;
	player.train(1);
	cout<<"4th training, capacity = "<<player.getCapacity(1)<<endl;
	player.train(1);
	cout<<"5th training, capacity = "<<player.getCapacity(1)<<endl;


	Broomstick broomstick = player.getBroomstick();
	cout<<"\nBroomstick capacity = "<<broomstick.getCapacityBoosted()<<endl;
	cout<<"Broomstick bonus = "<<broomstick.getBonus()<<endl;
	cout<<"Broomstick value = "<<broomstick.getValue()<<endl;

	cout<<"\nPlayer value = "<<player.getEstimatedValue()<<endl;


	cout<<"PlayingPlayer Creation : "<<endl;
	PlayingPlayer playingPlayer(player);
	cout<<"Capacities of the playing player : "<<endl;
	for (int i=0;i<5;++i) cout<<playingPlayer.getCapacity(i)<<endl;

	cout<<"Manager creation : "<<endl;
	Manager manager("Manager");
	for (int i=0;i<manager.getNumberOfPlayers();++i){
		manager.getPlayer(i).displayInformations(); //Pas très AMI mais juste pour tester et vérifier
		cout<<""<<endl;
	}

	Saver saver;

	saver.saveManager("Manager",manager);
	saver.savePlayer("Manager",manager.getPlayer(4));

	Manager manager2("Manager2");
	for (int i=0;i<manager2.getNumberOfPlayers();++i){
		manager2.getPlayer(i).displayInformations(); //Pas très AMI mais juste pour tester et vérifier
		cout<<i<<endl;
	}

	return 0;
}