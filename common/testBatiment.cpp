#include <iostream> // cin, cout...

#include "Building.hpp"


using namespace std;

int main() {
	cout<<"test de batiment"<<endl;
	//Building bat;
	Building bat2;
	Building bat(1);
	cout<<"prix pour evoluer:"<<bat.getPriceForNextLevel()<<endl;
	
	bat.upgradeLevel();
	
	
	
	cout<<"lvl gagner"<<bat._level<<endl;
	
	cout<<"prix pour evoluer:"<<bat.getPriceForNextLevel()<<endl;
	return 0;
}
