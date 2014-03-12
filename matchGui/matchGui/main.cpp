#include <QApplication>
#include <QPushButton>

#include <QHBoxLayout>
#include <QGridLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPolygonItem>
#include <QVector>
#include <QLabel>

#include <vector>

#include "fenetre.hpp"
#include "Defines.hpp"

#include "ManagedPlayer.hpp"
#include "PlayingPlayer.hpp"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	//création de team pour tester

	//COPIER-COLLER DE testMatch.cpp
/*	//MAIS probleme a la lecture des fichier
	std::vector<ManagedPlayer> team1, team2;
	team1.push_back(ManagedPlayer("Saves/defaultKeeper.txt")); //TEAM1_KEEPER
	team1.push_back(ManagedPlayer("Saves/defaultSeeker.txt")); //TEAM1_SEEKER
	team1.push_back(ManagedPlayer("Saves/defaultChaser.txt")); //TEAM1_CHASER1
	team1.push_back(ManagedPlayer("Saves/defaultChaser.txt"));//TEAM1_CHASER2
	team1.push_back(ManagedPlayer("Saves/defaultChaser.txt")); //TEAM1_CHASER3
	team1.push_back(ManagedPlayer("Saves/defaultBeater.txt")); //TEAM1_BEATER1
	team1.push_back(ManagedPlayer("Saves/defaultBeater.txt")); //TEAM1_BEATER2

	team2.push_back(ManagedPlayer("Saves/defaultKeeper.txt")); //TEAM2_KEEPER
	team2.push_back(ManagedPlayer("Saves/defaultSeeker.txt")); //TEAM2_SEEKER
	team2.push_back(ManagedPlayer("Saves/defaultChaser.txt")); //TEAM2_CHASER1
	team2.push_back(ManagedPlayer("Saves/defaultChaser.txt"));//TEAM2_CHASER2
	team2.push_back(ManagedPlayer("Saves/defaultChaser.txt")); //TEAM2_CHASER3
	team2.push_back(ManagedPlayer("Saves/defaultBeater.txt")); //TEAM2_BEATER1
	team2.push_back(ManagedPlayer("Saves/defaultBeater.txt")); //TEAM2_BEATER2	*/

	ManagedPlayer temp;
	std::vector<ManagedPlayer> team1, team2;
	temp.setCapacity(0,5);
	temp.setCapacity(1,5);
	temp.setCapacity(2,5);
	temp.setCapacity(3,5);
	temp.setCapacity(4,5);
	temp.setFirstName("Power");
	temp.setLastName("Prout");
	team1.push_back(temp); //TEAM1_KEEPER
	team1.push_back(temp); //TEAM1_SEEKER
	team1.push_back(temp); //TEAM1_CHASER1
	team1.push_back(temp);//TEAM1_CHASER2
	team1.push_back(temp); //TEAM1_CHASER3
	team1.push_back(temp); //TEAM1_BEATER1
	team1.push_back(temp); //TEAM1_BEATER2

	team2.push_back(temp); //TEAM2_KEEPER
	team2.push_back(temp); //TEAM2_SEEKER
	team2.push_back(temp); //TEAM2_CHASER1
	team2.push_back(temp);//TEAM2_CHASER2
	team2.push_back(temp); //TEAM2_CHASER3
	team2.push_back(temp); //TEAM2_BEATER1
	team2.push_back(temp); //TEAM2_BEATER2	*/


	//COPIER-COLLER de match.hpp -> attribut
	std::vector <PlayingPlayer> __players;
	std::vector <Ball> __balls;
	HexagonalField __field;
	//COPIER-COLLER de match.cpp -> constructeur match(...)
	for(int i = 0; i < 7; ++i){
		int role = i % TEAM2_KEEPER; //les 2 premiers roles correspondent déjà
		if((role < TEAM1_BEATER1) and (role > TEAM1_SEEKER)) {role = ROLE_CHASER;}
		else if((role < TEAM2_KEEPER) and (role > TEAM1_CHASER3)) {role = ROLE_BEATER;}
		AxialCoordinates position;
		switch(i){
			case TEAM1_KEEPER :
				position = AxialCoordinates(STARTINGDIAG_TEAM1_KEEPER, STARTINGLINE_TEAM1_KEEPER);
				break;
			case TEAM1_SEEKER :
				position = AxialCoordinates(STARTINGDIAG_TEAM1_SEEKER, STARTINGLINE_TEAM1_SEEKER);
				break;
			case TEAM1_CHASER1 :
				position = AxialCoordinates(STARTINGDIAG_TEAM1_CHASER1, STARTINGLINE_TEAM1_CHASER1);
				break;
			case TEAM1_CHASER2 :
				position = AxialCoordinates(STARTINGDIAG_TEAM1_CHASER2, STARTINGLINE_TEAM1_CHASER2);
				break;
			case TEAM1_CHASER3 :
				position = AxialCoordinates(STARTINGDIAG_TEAM1_CHASER3, STARTINGLINE_TEAM1_CHASER3);
				break;
			case TEAM1_BEATER1 :
				position = AxialCoordinates(STARTINGDIAG_TEAM1_BEATER1, STARTINGLINE_TEAM1_BEATER1);
				break;
			case TEAM1_BEATER2 :
				position = AxialCoordinates(STARTINGDIAG_TEAM1_BEATER2, STARTINGLINE_TEAM1_BEATER2);
				break;
		}
		__players.push_back(PlayingPlayer(team1[i], role, position));
		__field.setOccupant(position, i);
	}

	for(int i = 0; i < 7; ++i){ //création et positionnement de l'équipe 2
		int role = i % TEAM2_KEEPER; //les 2 premiers roles correspondent déjà
		if((role < TEAM1_BEATER1) and (role > TEAM1_SEEKER)) {role = ROLE_CHASER;}
		else if((role < TEAM2_KEEPER) and (role > TEAM1_CHASER3)) {role = ROLE_BEATER;}
		AxialCoordinates position;
		switch(i+TEAM2_KEEPER){
			case TEAM2_KEEPER :
				position = AxialCoordinates(STARTINGDIAG_TEAM2_KEEPER, STARTINGLINE_TEAM2_KEEPER);
				break;
			case TEAM2_SEEKER :
				position = AxialCoordinates(STARTINGDIAG_TEAM2_SEEKER, STARTINGLINE_TEAM2_SEEKER);
				break;
			case TEAM2_CHASER1 :
				position = AxialCoordinates(STARTINGDIAG_TEAM2_CHASER1, STARTINGLINE_TEAM2_CHASER1);
				break;
			case TEAM2_CHASER2 :
				position = AxialCoordinates(STARTINGDIAG_TEAM2_CHASER2, STARTINGLINE_TEAM2_CHASER2);
				break;
			case TEAM2_CHASER3 :
				position = AxialCoordinates(STARTINGDIAG_TEAM2_CHASER3, STARTINGLINE_TEAM2_CHASER3);
				break;
			case TEAM2_BEATER1 :
				position = AxialCoordinates(STARTINGDIAG_TEAM2_BEATER1, STARTINGLINE_TEAM2_BEATER1);
				break;
			case TEAM2_BEATER2 :
				position = AxialCoordinates(STARTINGDIAG_TEAM2_BEATER2, STARTINGLINE_TEAM2_BEATER2);
				break;
		}
		__players.push_back(PlayingPlayer(team2[i], role, position));
		__field.setOccupant(position, i+TEAM2_KEEPER);
	}
	//les 4 balles :
	__balls.push_back(Ball(GOLDENSNITCH, AxialCoordinates(STARTINGDIAG_GOLDENSNITCH, STARTINGLINE_GOLDENSNITCH)));
	__field.setOccupant(AxialCoordinates(STARTINGDIAG_GOLDENSNITCH, STARTINGLINE_GOLDENSNITCH), GOLDENSNITCH);
	__balls.push_back(Ball(BLUDGER1, AxialCoordinates(STARTINGDIAG_BLUDGER1, STARTINGLINE_BLUDGER1)));
	__field.setOccupant(AxialCoordinates(STARTINGDIAG_BLUDGER1, STARTINGLINE_BLUDGER1), BLUDGER1);
	__balls.push_back(Ball(GOLDENSNITCH, AxialCoordinates(STARTINGDIAG_BLUDGER2, STARTINGLINE_BLUDGER2)));
	__field.setOccupant(AxialCoordinates(STARTINGDIAG_BLUDGER2, STARTINGLINE_BLUDGER2), BLUDGER2);
	__balls.push_back(Ball(GOLDENSNITCH, AxialCoordinates(STARTINGDIAG_QUAFFLE, STARTINGLINE_QUAFFLE)));
	__field.setOccupant(AxialCoordinates(STARTINGDIAG_QUAFFLE, STARTINGLINE_QUAFFLE), QUAFFLE);



//	fenetre test; //pour des test
//	test.show();

	fenetre test2(__field,__players,__balls);
	test2.show();


    return app.exec();
}
