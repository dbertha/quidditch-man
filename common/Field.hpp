#ifndef FIELD_HPP
#define FIELD_HPP

#include "PositionOnField.hpp"

class Field {
private :
    int __matrices[2][MATRIX_SIZE][MATRIX_SIZE];
public :
    Field();
    int getOccupant(PositionOnField aPosition);
};

#endif
