#ifndef HEXAGONALFIELD_HPP
#define HEXAGONALFIELD_HPP

#include "Coordinates.hpp"
#include "Defines.hpp"
#include <vector>
#include <string>




//TODO : affichage du vif d'or seulement si l'attrapeur de l'équipe est dans les environs
//TODO : corriger defaut affichage avec unicodes


typedef int objectIndex_t;

class HexagonalField {
private :
    objectIndex_t matrix[MATRIX_SIZE][MATRIX_SIZE];
    std::vector<std::string> unicodes;
public :
    HexagonalField();
    int getOccupant(AxialCoordinates coord) const;
    bool setOccupant(AxialCoordinates coord, objectIndex_t object);
    void display();
    void display(AxialCoordinates selected, int distance);
    bool isFree(AxialCoordinates toTest);
    void reset();
};

#endif
