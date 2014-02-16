#ifndef HEXAGONALFIELD_HPP
#define HEXAGONALFIELD_HPP

#include "Coordinates.hpp"

class HexagonalField {
private :
    int matrix[MATRIX_SIZE][MATRIX_SIZE];
public :
    HexagonalField();
    int getOccupant(AxialCoordinates coord);
    void display();
};

#endif
