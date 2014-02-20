#ifndef BALL_HPP
#define BALL_HPP

//~ Rappel, dans HexagonalField.hpp :
//~ #define GOLDENSNITCH 14
//~ #define BLUDGER1 15
//~ #define BLUDGER2 16
//~ #define QUAFFLE 17

//TODO : role inutile ?

class Ball{
public :
    int getRole(){return __role;}
    AxialCoordinates getPosition(){return __currentPosition;}
    Ball(int role, AxialCoordinates startingPosition) : __role(role), __currentPosition(startingPosition) {}
    void moveTo(AxialCoordinates destination) {__currentPosition = destination;}
private :
    int __role;
    AxialCoordinates __currentPosition;
};
#endif
