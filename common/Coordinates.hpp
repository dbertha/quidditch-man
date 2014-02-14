#ifndef COORDINATES_HPP
#define COORDINATES_HPP

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
// axe x correspond aux colonnes
// axe y aux rangées
private :
    int __row;
    int __col;
public :
    AxialCoordinates(int row, int col) : __row(row), __col(col) {} //attention, row et col ne sont pas les mêmes que les indices de la matrice, ici ils désignent le décalage par rapport au centre : 
    //For hexagon shaped maps of radius N, where N = max(abs(x), abs(y), abs(z), we have first_column[row] == -N - min(0, row). 
    //You’d access array[row][col + N + min(0, r)]. However, since we’re starting with some values of r < 0, we also have to offset the row, and use array[row + N][col + N + min(0, r)].
    AxialCoordinates(CubicCoordinates cubicCoord);
    int getRow() {return __row;}
    int getCol() {return __col;}
    AxialCoordinates[6] getNeighbors;
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
    CubicCoordinates(AxialCoordinates axialCoord) : __xCoord(axialCoord.getCol()), __yCoord(-axialCoord.getCol() - axialCoord.getRow()), __zCoord(axialCoord.getRow());
    int getXCoord() {return __xCoord;}
    int getYCoord() {return __yCoord;}
    int getZCoord() {return __zCoord;}
    CubicCoordinates[6] getNeighbors;
    //~ neighbors = [
   //~ [+1, -1,  0], [+1,  0, -1], [ 0, +1, -1],
   //~ [-1, +1,  0], [-1,  0, +1], [ 0, -1, +1]
//~ ]
//~ d = neighbors[direction]
//~ return CubicCoordinates(x + d[0], y + d[1], z + d[2])
    //TODO : conversion par assignation
};


AxialCoordinates::AxialCoordinates(CubicCoordinates cubicCoord) : __row(cubicCoord.getZCoord()), __col(cubicCoord.getXCoord()) {}


#endif
