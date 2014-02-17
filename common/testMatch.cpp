#include "Coordinates.hpp"
#include "HexagonalField.hpp"
#include <iostream>

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
    
    std::cout << "Test de la récupération des déplacements : " << std::endl;
    std::list<Move> moves = axialCoord.getMovesTo(AxialCoordinates(-2,4));
    std::cout << moves.size() << std::endl;
    for(std::list<Move>::iterator list_iter = moves.begin(); list_iter != moves.end(); list_iter++){
        std::cout << (*list_iter).getDiagDiff() << " " << (*list_iter).getLineDiff() << std::endl;
    }
    
}
