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
	cout<<player.getPopularity()<<endl;
	cout<<player.isBlocked()<<endl;

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

	return 0;
}