#include <iostream> // cin, cout...

#include "Building.hpp"
#include "TrainingCenter.hpp"
#include "Hospital.hpp"
#include "Stadium.hpp"
#include "ManagedPlayer.hpp"
#include "FanShop.hpp"
#include "Manager.hpp"
#include "Saver.hpp"
#include "RecruitmentCenter.hpp"

using namespace std;

int main() {
	
	Stadium stade(1,100000,500);
	cout<<stade.getLevel()<<endl;
	cout<<stade.getMaxPlaces()<<endl;
	cout<<stade.getPriceForNextLevel()<<endl;

	stade.upgradeBuilding();


	cout<<stade.getLevel()<<endl;
	cout<<stade.getMaxPlaces()<<endl;
	cout<<stade.getPriceForNextLevel()<<endl;

	ManagedPlayer player("Saves/defaultSeeker.txt");
	player.displayInformations();
	TrainingCenter trainingCenter(1,75000,7);
	cout<<trainingCenter.getTimeRequired()<<endl;
	for (int i=0;i<6;++i) trainingCenter.train(player,0);
	player.displayInformations();

	player.unlockPlayer();
	player.setLife(0);
	player.displayInformations();
	Hospital hospital(3,75000,7);
	cout<<hospital.getTimeRequired()<<endl;
	hospital.heal(player);
	player.displayInformations();


	FanShop fanshop(3, 50000, 100);
	Manager manager("Manager");
	cout<<manager.getMoney()<<endl;
	fanshop.getIncome(manager);
	cout<<manager.getMoney()<<endl;



	ManagedPlayer player2("Saves/defaultBeater.txt");
	cout<<player2.getFirstName()<<" "<<player2.getLastName()<<endl;
	RecruitmentCenter rc(1,90000);
	rc.sellPlayer(manager,player2,500000);




/*
	cout<<"test de batiment"<<endl;
	//Building bat;
	Building bat2;
	Building bat(1);
	cout<<"prix pour evoluer:"<<bat.getPriceForNextLevel()<<endl;
	
	bat.upgradeLevel();
	
	
	
	cout<<"lvl gagner"<<bat._level<<endl;
	
	cout<<"prix pour evoluer:"<<bat.getPriceForNextLevel()<<endl;
	
	cout<<endl<<"------------Test entrainement---------"<<endl<<endl;
	TrainingCenter trainer;
	ManagedPlayer joueur("defaultBeater.txt");
	cout<<"niveau de speed "<<joueur.getCapacity(0)<<endl;
	cout<<"nbr entrainement restant "<<joueur.getTrainingLeft(0)<<endl;
	cout<<"prix entrainement "<<trainer.getPriceToTrainCaract(joueur,0)<<endl;
	cout<<"joueur bloquer = "<<joueur.isBlocked()<<endl;
	trainer.train(joueur,0);
	cout<<"*** 1 entrainement fait"<<endl;
	cout<<"niveau de speed "<<joueur.getCapacity(0)<<endl;
	cout<<"nbr entrainement restant "<<joueur.getTrainingLeft(0)<<endl;
	cout<<"prix entrainement "<<trainer.getPriceToTrainCaract(joueur,0)<<endl;
	cout<<"joueur bloquer = "<<joueur.isBlocked()<<endl;
	trainer.train(joueur,0);
	cout<<"*** 2 entrainement fait"<<endl;
	cout<<"niveau de speed "<<joueur.getCapacity(0)<<endl;
	cout<<"nbr entrainement restant "<<joueur.getTrainingLeft(0)<<endl;
	cout<<"prix entrainement "<<trainer.getPriceToTrainCaract(joueur,0)<<endl;
	cout<<"joueur bloquer = "<<joueur.isBlocked()<<endl;
	trainer.train(joueur,0);
	cout<<"*** 3 entrainement fait"<<endl;
	cout<<"niveau de speed "<<joueur.getCapacity(0)<<endl;
	cout<<"nbr entrainement restant "<<joueur.getTrainingLeft(0)<<endl;
	cout<<"prix entrainement "<<trainer.getPriceToTrainCaract(joueur,0)<<endl;
	cout<<"joueur bloquer = "<<joueur.isBlocked()<<endl;
	cout<<endl;
	cout<<"niveau du centre d'entrainement "<<trainer.getLevel()<<endl;
	cout<<"prix pour upgrade le batiement "<<trainer.getPriceForNextLevel()<<endl;
	cout<<"prix entrainement "<<trainer.getPriceToTrainCaract(joueur,0)<<endl;
	trainer.upgradeLevel();
	cout<<"niveau du centre d'entrainement "<<trainer.getLevel()<<endl;
	cout<<"prix pour upgrade le batiement "<<trainer.getPriceForNextLevel()<<endl;
	cout<<"prix entrainement "<<trainer.getPriceToTrainCaract(joueur,0)<<endl;
	trainer.upgradeLevel();
	cout<<"niveau du centre d'entrainement "<<trainer.getLevel()<<endl;
	cout<<"prix pour upgrade le batiement "<<trainer.getPriceForNextLevel()<<endl;
	cout<<"prix entrainement "<<trainer.getPriceToTrainCaract(joueur,0)<<endl;

*/
	return 0;
}
