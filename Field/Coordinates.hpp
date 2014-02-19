#ifndef COORDINATES_HPP
#define COORDINATES_HPP

#include <cstdlib> // abs()
#include <cmath> //floor()
#include <algorithm> //min() 

#define MATRIX_SIZE 27 // nombre impair pour obtenir un centre unique en [MATRIX_SIZE/2][MATRIX_SIZE/2], qui correspondra à [0][0] en coordonnées axiales

//Q : classe parente pour utiliser le polymorphisme ?

class CubicCoordinates;
//there are three primary axes for hexagonal grids, unlike the two we have for square grids. There’s an elegant symmetry with these.
//Let’s take a cube grid and slice out a diagonal plane at x + y + z = 0.
//Notice the three primary axes on the cube grid, and how they correspond to six hex grid diagonal directions; the diagonal grid axes corresponds to a primary hex grid direction.
//Because we already have algorithms for square and cube grids, using cube coordinates allows us to adapt those algorithms to hex grids.

class AxialCoordinates {
//The 2-axis coordinate system, sometimes called “trapezoidal”, 
//uses two of the three coordinates from the cube (3-axis) coordinate system. 
//Since we have the constraint x + y + z = 0, given any two of those, we can generate the third.
// axe z : firstAxis (r)
// axe x  : secondAxis (q) 
private :
    int __firstAxis;
    int __secondAxis;
public :
    AxialCoordinates(int firstAxis, int secondAxis) : __firstAxis(firstAxis), __secondAxis(secondAxis) {} //attention, firstAxis et secondAxis ne sont pas les mêmes que les indices de la matrice, 
    //ici ils désignent le décalage par rapport au centre : 
    //For hexagon shaped maps of radius N, where N = max(abs(x), abs(y), abs(z), we have first_column[row] == -N - min(0, row). 
    //You’d access array[row][col + N + min(0, r)]. However, since we’re starting with some values of r < 0, we also have to offset the row, and use array[row + N][col + N + min(0, r)].
    AxialCoordinates(CubicCoordinates cubicCoord);
    int getFirstAxis() const {return __firstAxis;}
    int getSecondAxis() const {return __secondAxis;}
    bool isNeighborOf(const AxialCoordinates &potentialNeighbor) const {
        bool result = false;
        int firstAxisDiff = this->getFirstAxis() - potentialNeighbor.getFirstAxis();
        int secondAxisDiff = this->getSecondAxis() - potentialNeighbor.getSecondAxis();
        if((abs(firstAxisDiff) < 2) and (abs(secondAxisDiff) < 2) and (abs(firstAxisDiff + secondAxisDiff) < 2)){
            result = true;
        }
        return result;
    }
    bool isDirection(const int direction[2]) const {
        bool result = false;
        if((abs(direction[0]) < 2) and (abs(direction[1]) < 2) and (abs(direction[0] + direction[1]) < 2)){
            result = true;
        }
        return result;
    }
    AxialCoordinates getNeighbor(const int direction[2]) const{
        //on suppose la direction ok
        return AxialCoordinates(this->getFirstAxis() - direction[0], this->getSecondAxis() - direction[1]);
    }
    
    int getLineOnMatrix() const { // r + N
        return (getFirstAxis() + (MATRIX_SIZE / 2)); 
        //Q : la division va déjà redonner un entier : quel arrondi ? ==> vers le bas, floor inutile
    }
    
    int getColOnMatrix() const { // q + N (+ min(0, r) : étrange)
        return (getSecondAxis() + (MATRIX_SIZE / 2));
    }
    
    
    
    
    //TODO : tester sortie hors du terrain
    
    
    //AxialCoordinates[6] getNeighbors;
    //~ neighbors = [
   //~ [+1,  0], [+1, -1], [ 0, -1],
   //~ [-1,  0], [-1, +1], [ 0, +1]
//~ ]
//~ d = neighbors[direction]
//~ return AxialCoordinates(q + d[0], r + d[1])

    //TODO : conversion par assignation
};

class CubicCoordinates {
private :
    int __xCoord;
    int __yCoord;
    int __zCoord;
public :
    CubicCoordinates(int xCoord, int yCoord, int zCoord) : __xCoord(xCoord), __yCoord(yCoord), __zCoord(zCoord) {}
    CubicCoordinates(AxialCoordinates axialCoord) : __xCoord(axialCoord.getSecondAxis()), __yCoord(-axialCoord.getSecondAxis() - axialCoord.getFirstAxis()), __zCoord(axialCoord.getFirstAxis()) {}
    int getXCoord() {return __xCoord;}
    int getYCoord() {return __yCoord;}
    int getZCoord() {return __zCoord;}
    //CubicCoordinates[6] getNeighbors;
    //~ neighbors = [
   //~ [+1, -1,  0], [+1,  0, -1], [ 0, +1, -1],
   //~ [-1, +1,  0], [-1,  0, +1], [ 0, -1, +1]
//~ ]
//~ d = neighbors[direction]
//~ return CubicCoordinates(x + d[0], y + d[1], z + d[2])
    //TODO : conversion par assignation
};


//AxialCoordinates::AxialCoordinates(CubicCoordinates cubicCoord) : __firstAxis(cubicCoord.getZCoord()), __secondAxis(cubicCoord.getXCoord()) {}


#endif
