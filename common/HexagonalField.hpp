#ifndef HEXAGONALFIELD_HPP
#define HEXAGONALFIELD_HPP

#include "Coordinates.hpp"

#define TEAM1_KEEPER 0
#define TEAM1_SEEKER 1
#define TEAM1_CHASER1 2
#define TEAM1_CHASER2 3
#define TEAM1_CHASER3 4
#define TEAM1_BEATER1 5
#define TEAM1_BEATER2 6
#define TEAM2_KEEPER 7
#define TEAM2_SEEKERT 8
#define TEAM2_CHASER1 9
#define TEAM2_CHASER2 10
#define TEAM2_CHASER3 11
#define TEAM2_BEATER1 12
#define TEAM2_BEATER2 13
#define GOLDENSNITCH 14
#define BLUDGER1 15
#define BLUDGER2 16
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
