#include "PositionOnField.hpp"
#include <iostream>
#include <cstdlib>


PositionOnField::PositionOnField(unsigned int numOfMatrix, unsigned int row, unsigned int line) : __numOfMatrix(numOfMatrix), __line(line), __row(row) {
    if((numOfMatrix > 2) or (numOfMatrix < 1)
        or (line >=  MATRIX_SIZE) or (row >=  MATRIX_SIZE)){
        std::cerr << "Erreur dans la construction de la PositionOfField" << std::endl;
    }
}
unsigned int PositionOnField::getNumOfMatrix() const {return __numOfMatrix;}
unsigned int PositionOnField::getRow() const {return __row;}
unsigned int PositionOnField::getLine() const {return __line;}

int main(){
    PositionOnField firstPosition(1, 12, 13);
    return EXIT_SUCCESS;
}
