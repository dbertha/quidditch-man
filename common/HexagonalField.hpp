#ifndef HEXAGONALFIELD_HPP
#define HEXAGONALFIELD_HPP

#include "Coordinates.hpp"

#define KEEPERTEAM1 00
#define SEEKERTEAM1 01
#define BEATERTEAM1 02
#define CHASER1TEAM1 03
#define CHASER2TEAM1 04
#define BEATER1TEAM1 05
#define BEATER2TEAM1 06
#define KEEPERTEAM2 07
#define SEEKERTEAM2 08
#define BEATERTEAM2 09
#define CHASER1TEAM2 10
#define CHASER2TEAM2 11
#define BEATER1TEAM2 12
#define BEATER2TEAM2 13
#define GOLDENSNITCH 14
#define BLUDGER1 15
#define BLUDGER12 16
#define QUAFFLE 17

//~ #define KEEPERTEAM1_UNICODE 0
//~ #define SEEKERTEAM1_UNICODE 1
//~ #define BEATERTEAM1_UNICODE 2
//~ #define CHASER1TEAM1_UNICODE 3
//~ #define CHASER2TEAM1_UNICODE 4
//~ #define BEATER1TEAM1_UNICODE 5
//~ #define BEATER2TEAM1_UNICODE 6
//~ #define KEEPERTEAM2_UNICODE 7
//~ #define SEEKERTEAM2_UNICODE 8
//~ #define BEATERTEAM2_UNICODE 9
//~ #define CHASER1TEAM2_UNICODE 10
//~ #define CHASER2TEAM2_UNICODE 11
//~ #define BEATER1TEAM2_UNICODE 12
//~ #define BEATER2TEAM2_UNICODE 13
//~ #define GOLDENSNITCH_UNICODE 14
//~ #define BLUDGER_UNICODE 15
//~ #define QUAFFLE_UNICODE 17


typedef int objectIndex_t;

class HexagonalField {
private :
    objectIndex_t matrix[MATRIX_SIZE][MATRIX_SIZE];
public :
    HexagonalField();
    int getOccupant(AxialCoordinates coord) const;
    bool setOccupant(AxialCoordinates coord, objectIndex_t object);
    void display();
    void display(AxialCoordinates selected, int distance);
};

#endif
