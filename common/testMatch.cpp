#include "Coordinates.hpp"
#include "HexagonalField.hpp"
#include "ManagedPlayer.hpp"
#include "PlayingPlayer.hpp"
#include "Match.hpp"
#include <iostream>
#include <vector>

//~ g++ -Wall -Wextra HexagonalField.cpp Coordinates.hpp PlayingPlayer.cpp Player.cpp ManagedPlayer.cpp Broomstick.cpp testMatch.cpp Match.cpp -o match.out


int main(){
    AxialCoordinates axialCoord(0,0);
    AxialCoordinates axialCoord2(1,0), axialCoord3(1, 1), axialCoord4(-1, 1);
    //~ std::cout << axialCoord.isNeighborOf(axialCoord2) << std::endl;
    //~ std::cout << axialCoord.isNeighborOf(axialCoord3) << std::endl;
    //~ std::cout << axialCoord.isNeighborOf(axialCoord4) << std::endl;
    //int direction[2] = {1, 0};
    //AxialCoordinates axialCoord5 = axialCoord.getNeighbor(direction);
    
    //~ std::cout << axialCoord.isNeighborOf(axialCoord5) << std::endl;
    //~ 
    //~ std::cout << axialCoord.getLineOnMatrix() << " " << axialCoord.getColOnMatrix() << std::endl;
    //~ 
    //~ std::cout << axialCoord2.getLineOnMatrix() << " " << axialCoord2.getColOnMatrix() << std::endl;
    //~ 
    //~ std::cout << axialCoord3.getLineOnMatrix() << " " << axialCoord3.getColOnMatrix() << std::endl;
    //~ 
    //~ std::cout << axialCoord4.getLineOnMatrix() << " " << axialCoord4.getColOnMatrix() << std::endl;
    
    HexagonalField field;
    //field.display();
    
    field.setOccupant(AxialCoordinates(0,0), TEAM1_CHASER1);
    std::cout << field.setOccupant(AxialCoordinates(0,1), TEAM1_CHASER2) << std::endl;
    std::cout << field.setOccupant(AxialCoordinates(5,-5), TEAM2_CHASER1) << std::endl;
    field.display();
    field.display(AxialCoordinates(0,1), 2);
    
    std::cout << axialCoord.getDistanceTo(AxialCoordinates(0,1)) << std::endl; // doit donner 1
    std::cout << axialCoord.getDistanceTo(AxialCoordinates(1,1)) << std::endl; // doit donner 2
    std::cout << axialCoord3.getDistanceTo(AxialCoordinates(1,1)) << std::endl; // doit donner 0
    std::cout << axialCoord3.getDistanceTo(axialCoord) << std::endl; // doit donner 2
    std::cout << axialCoord.getDistanceTo(AxialCoordinates(-1,-1)) << std::endl; // doit donner 2
    std::cout << axialCoord.getDistanceTo(AxialCoordinates(-2,-1)) << std::endl; // doit donner 3
    //field.display(AxialCoordinates(5,-5), 3);
    Move firstMove(1,-1), secondMove(axialCoord, axialCoord2);
    std::cout << firstMove.getDiagDiff() << std::endl; // doit donner 1
    std::cout << firstMove.getLineDiff() << std::endl; // -1
    std::cout << secondMove.getDiagDiff() << std::endl; // doit donner 1
    std::cout << secondMove.getLineDiff() << std::endl; //doit donner 0
    //~ 
    //~ std::cout << "Test de la récupération des déplacements : " << std::endl;
    //~ std::list<Move> moves = axialCoord.getMovesTo(AxialCoordinates(-2,4));
    //~ std::cout << moves.size() << std::endl;
    //~ for(std::list<Move>::iterator list_iter = moves.begin(); list_iter != moves.end(); list_iter++){
        //~ std::cout << (*list_iter).getDiagDiff() << " " << (*list_iter).getLineDiff() << std::endl;
    //~ }
    ManagedPlayer chaser("defaultChaser.txt");
    PlayingPlayer myPlayer(chaser, ROLE_CHASER, axialCoord);
    std::cout << myPlayer.getMaxDistance() << " " << chaser.getCapacity(SPEED) << std::endl;
    std::cout << myPlayer.interceptQuaffle(4) << " " << myPlayer.catchGoldenSnitch(3) << std::endl;
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
    team2.push_back(ManagedPlayer("Saves/defaultBeater.txt")); //TEAM2_BEATER2
    
    int movesTeam1[7][4], movesTeam2[7][4];
    movesTeam1[0][0] = TEAM1_KEEPER;
    movesTeam1[0][1] = 0;
    movesTeam1[0][2] = -7;
    movesTeam1[0][3] = 0;
    movesTeam2[0][0] = TEAM2_KEEPER;
    movesTeam2[0][1] = 0;
    movesTeam2[0][2] = 7;
    movesTeam2[0][3] = 0;
    for(int i = 1; i < 7; ++i){
        movesTeam1[i][0] = i;
        movesTeam1[i][1] = 0;
        movesTeam1[i][2] = 10000;
        movesTeam1[i][3] = 10000;
        movesTeam2[i][0] = i+7;
        movesTeam2[i][1] = 0;
        movesTeam2[i][2] = 10000;
        movesTeam2[i][3] = 10000;
    }
    
    Match theMatch(team1, team2);
    theMatch.makeMoves(movesTeam1, movesTeam2);
    
}
