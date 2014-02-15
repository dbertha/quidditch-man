#include "Manager.hpp"
#include "Player.hpp"
#include "ManagedPlayer.hpp"
#include "PlayingPlayer.hpp"
#include "Broomstick.hpp"

#include <iostream>

using namespace std;


int main() {
	
	ManagedPlayer player("defaultSeeker.txt");
	cout<<player.getFirstName()<<" "<<player.getLastName()<<endl;
	for (int i=0;i<5;++i){
		cout<<player.getCapacity(i)<<endl;
	}
	cout<<"Popularity : "<<player.getPopularity()<<endl;
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

	cout<<"\nGain popularity"<<endl;
	cout<<"Popularity = "<<player.getPopularity()<<endl;
	player.gainPopularity();
	cout<<"After gainPopularity = "<<player.getPopularity()<<endl;

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


	return 0;
}