#include "Coordinates.hpp"
#include "HexagonalField.hpp"
#include <iostream>

int main(){
    AxialCoordinates axialCoord(0,0);
    AxialCoordinates axialCoord2(1,0), axialCoord3(1, 1), axialCoord4(-1, 1);
    //~ std::cout << axialCoord.isNeighborOf(axialCoord2) << std::endl;
    //~ std::cout << axialCoord.isNeighborOf(axialCoord3) << std::endl;
    //~ std::cout << axialCoord.isNeighborOf(axialCoord4) << std::endl;
    int direction[2] = {1, 0};
    AxialCoordinates axialCoord5 = axialCoord.getNeighbor(direction);
    
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
    field.display();
    
    field.setOccupant(AxialCoordinates(0,0), CHASER1TEAM1);
    std::cout << field.setOccupant(AxialCoordinates(0,1), CHASER2TEAM1) << std::endl;
    std::cout << field.setOccupant(AxialCoordinates(5,-5), CHASER1TEAM2) << std::endl;
    field.display();
    
}
