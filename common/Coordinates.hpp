#ifndef COORDINATES_HPP
#define COORDINATES_HPP

#include <cstdlib> // abs()
#include <cmath> //floor()
#include <algorithm> //min() 
#include <list>

#define MATRIX_SIZE 23 // nombre impair pour obtenir un centre unique en [MATRIX_SIZE/2][MATRIX_SIZE/2], qui correspondra à [0][0] en coordonnées axiales

//Q : classe parente pour utiliser le polymorphisme ?

class CubicCoordinates;
//there are three primary axes for hexagonal grids, unlike the two we have for square grids. There’s an elegant symmetry with these.
//Let’s take a cube grid and slice out a diagonal plane at x + y + z = 0.
//Notice the three primary axes on the cube grid, and how they correspond to six hex grid diagonal directions; the diagonal grid axes corresponds to a primary hex grid direction.
//Because we already have algorithms for square and cube grids, using cube coordinates allows us to adapt those algorithms to hex grids.

class AxialCoordinates;

class Move;


class AxialCoordinates {
//The 2-axis coordinate system, sometimes called “trapezoidal”, 
//uses two of the three coordinates from the cube (3-axis) coordinate system. 
//Since we have the constraint x + y + z = 0, given any two of those, we can generate the third.
// axe z : diagAxis (r)
// axe x  : lineAxis (q) 
private :
    int __diagAxis;
    int __lineAxis;
public :
    AxialCoordinates(int diagAxis=0, int lineAxis=0) : __diagAxis(diagAxis), __lineAxis(lineAxis) {} //attention, diagAxis et lineAxis ne sont pas les mêmes que les indices de la matrice, 
    //ici ils désignent le décalage par rapport au centre : 
    //For hexagon shaped maps of radius N, where N = max(abs(x), abs(y), abs(z), we have first_column[row] == -N - min(0, row). 
    //You’d access array[row][col + N + min(0, r)]. However, since we’re starting with some values of r < 0, we also have to offset the row, and use array[row + N][col + N + min(0, r)].
    AxialCoordinates(CubicCoordinates cubicCoord);
    int getDiagAxis() const {return __diagAxis;}
    int getLineAxis() const {return __lineAxis;}
    bool isNeighborOf(const AxialCoordinates &potentialNeighbor) const {
        bool result = false;
        int diagAxisDiff = this->getDiagAxis() - potentialNeighbor.getDiagAxis();
        int lineAxisDiff = this->getLineAxis() - potentialNeighbor.getLineAxis();
        if((abs(diagAxisDiff) < 2) and (abs(lineAxisDiff) < 2) and (abs(diagAxisDiff + lineAxisDiff) < 2)){
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
        return AxialCoordinates(this->getDiagAxis() - direction[0], this->getLineAxis() - direction[1]);
    }
    
    int getLineOnMatrix() const { // r + N
        return (getLineAxis() + (MATRIX_SIZE / 2)); 
        //Q : la division va déjà redonner un entier : quel arrondi ? ==> vers le bas, floor inutile
    }
    
    int getColOnMatrix() const { // q + N (+ min(0, r) : étrange)
        return (getDiagAxis() + (MATRIX_SIZE / 2));
    }
    
    bool isOnField(){
        return (abs(getDiagAxis() + getLineAxis()) < (MATRIX_SIZE/2)) //sur la partie de la matrice utilisée
        and (abs(getDiagAxis()) < (MATRIX_SIZE/2)) //out of range
        and (abs(getLineAxis()) < (MATRIX_SIZE/2));
    }
    
    int getDistanceTo(AxialCoordinates destination){
        //calcul des coordonnées cubiques => méthodes de CubicCoord ?
        int x1 = getLineAxis();
        int z1 = getDiagAxis();
        int x2 = destination.getLineAxis();
        int z2 = destination.getDiagAxis();
        int y1 = -(x1 + z1);
        int y2 = -(x2 + z2);
        return (abs(x1 - x2) + abs(y1 - y2) + abs(z1 - z2)) / 2;
    }
    
    inline std::list<Move> getMovesTo(AxialCoordinates destination);
    
    
    
    
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
    CubicCoordinates(AxialCoordinates axialCoord) : __xCoord(axialCoord.getLineAxis()), __yCoord(-axialCoord.getLineAxis() - axialCoord.getDiagAxis()), __zCoord(axialCoord.getDiagAxis()) {}
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

class Move {
    //Déplacement d'1 seule case
    //possibilités : [+1, -1], [-1,+1], [+1, 0], [0,+1], [-1,0], [0,-1]
    //TODO : gérer par enum ou vecteur : sécurité : pas de mouvement non autorisés
private:
    int __moveOnDiag; //diag case destination - diag case départ
    int __moveOnLine; //line case destination - line case départ
public:
    Move(int moveOnDiag, int moveOnLine) : __moveOnDiag(moveOnDiag), __moveOnLine(moveOnLine){}
    Move(const AxialCoordinates &start, const AxialCoordinates &end) : __moveOnDiag(end.getDiagAxis() - start.getDiagAxis()), __moveOnLine(end.getLineAxis() - start.getLineAxis()) {}
    int getDiagDiff(){return __moveOnDiag;}
    int getLineDiff(){return __moveOnLine;}
};

std::list<Move> AxialCoordinates::getMovesTo(AxialCoordinates destination){
    //TODO : éviter de faire une copie lors du return
    int diagDiff = destination.getDiagAxis() - getDiagAxis();
    int lineDiff = destination.getLineAxis() - getLineAxis();
    std::list<Move> orderedMoves;
    while((diagDiff != 0) or (lineDiff != 0)){
        //le long diagonale opposée
        if((diagDiff > 0) and (lineDiff < 0)){
            orderedMoves.push_back(Move(1,-1));
            diagDiff -= 1;
            lineDiff -= -1;
        }
        else if((diagDiff < 0) and (lineDiff > 0)){
            orderedMoves.push_back(Move(-1,1));
            diagDiff -= -1;
            lineDiff -= 1;
        }
        //le long de la diagonale
        else if(abs(diagDiff) > abs(lineDiff)){
            orderedMoves.push_back(Move(diagDiff/abs(diagDiff), 0));
            diagDiff -= diagDiff/abs(diagDiff);
        }
        //le long de la ligne
        else{
            orderedMoves.push_back(Move(0, lineDiff/abs(lineDiff)));
            lineDiff -= lineDiff/abs(lineDiff); 
        }
            
    }
    return orderedMoves;
}

#endif
