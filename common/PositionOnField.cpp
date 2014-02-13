#include "PositionOnField.hpp"
#include <iostream>
#include <cstdlib>


PositionOnField::PositionOnField(unsigned int numOfMatrix,  unsigned int row, unsigned int col) : __numOfMatrix(numOfMatrix), __row(row), __col(col) {
    if((numOfMatrix > 1) or (row >=  MATRIX_SIZE) or (col >=  MATRIX_SIZE)){
        std::cerr << "Erreur dans la construction de la PositionOfField" << std::endl;
    }
}

unsigned int PositionOnField::getNumOfMatrix() const {return __numOfMatrix;}
unsigned int PositionOnField::getCol() const {return __col;}
unsigned int PositionOnField::getRow() const {return __row;}


//~ int main(){
    //~ PositionOnField firstPosition(1, 12, 13);
    //~ return EXIT_SUCCESS;
//~ }
