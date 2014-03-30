#ifndef HEXAGONALFIELD_HPP
#define HEXAGONALFIELD_HPP

#include "Coordinates.hpp"
#include "Defines.hpp"
#include <vector>
#include <string>




//évolution éventuelle : affichage du vif d'or seulement si l'attrapeur de l'équipe est dans les environs

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
    bool isGoal(int rowOnMatrix, int colOnMatrix);
    bool isGoal(AxialCoordinates toTest);
    bool isTeam2Goal(AxialCoordinates toTest);
    bool isTeam1Goal(AxialCoordinates toTest);
    void reset();
};

#endif
