#include "Player.hpp"
#include "ManagedPlayer.hpp"
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


	return 0;
}