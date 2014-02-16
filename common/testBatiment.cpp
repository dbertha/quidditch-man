#include <iostream> // cin, cout...

#include "Building.hpp"
#include "TrainingCenter.hpp"


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
	return 0;
}
