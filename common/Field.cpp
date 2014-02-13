#include "PositionOnField.hpp"
#include "Field.hpp"

#include <iostream>
#include <cstdlib>

Field::Field(){
    for(int indexMatrix = 0; indexMatrix < 2; ++indexMatrix){
        for (int indexRow = 0; indexRow < MATRIX_SIZE; ++indexRow){
            for (int indexCol = 0; indexCol < MATRIX_SIZE; ++indexCol){
                __matrices[indexMatrix][indexRow][indexCol] = -1; // -1 : position inoccupée
            }
        }
    }
}

int Field::getOccupant(PositionOnField aPosition){
    return __matrices[aPosition.getNumOfMatrix()][aPosition.getRow()][aPosition.getCol()];
}

int main(){
    Field aField;
    PositionOnField aPosition(0,5,13);
    std::cout << "Occupant : " << aField.getOccupant(aPosition) << std::endl;
    return EXIT_SUCCESS;
}
