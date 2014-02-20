#ifndef HEXAGONALFIELD_HPP
#define HEXAGONALFIELD_HPP

#include "Coordinates.hpp"
#include <vector>
#include <string>

#define NOT_ON_HEX_GRID -2
#define FREE_SPACE -1
#define TEAM1_KEEPER 0
#define TEAM1_SEEKER 1
#define TEAM1_CHASER1 2
#define TEAM1_CHASER2 3
#define TEAM1_CHASER3 4
#define TEAM1_BEATER1 5
#define TEAM1_BEATER2 6
#define TEAM2_KEEPER 7
#define TEAM2_SEEKER 8
#define TEAM2_CHASER1 9
#define TEAM2_CHASER2 10
#define TEAM2_CHASER3 11
#define TEAM2_BEATER1 12
#define TEAM2_BEATER2 13
#define GOLDENSNITCH 14
#define BLUDGER1 15
#define BLUDGER2 16
#define QUAFFLE 17

#define TEAM1_KEEPER_UNICODE "\u2780"
#define TEAM1_SEEKER_UNICODE "\u2781"
#define TEAM1_CHASER1_UNICODE "\u2782"
#define TEAM1_CHASER2_UNICODE "\u2783"
#define TEAM1_CHASER3_UNICODE "\u2784"
#define TEAM1_BEATER1_UNICODE "\u2785"
#define TEAM1_BEATER2_UNICODE "\u2786"
#define TEAM2_KEEPER_UNICODE "\u278A"
#define TEAM2_SEEKER_UNICODE "\u278B"
#define TEAM2_CHASER1_UNICODE "\u278C"
#define TEAM2_CHASER2_UNICODE "\u278D"
#define TEAM2_CHASER3_UNICODE "\u278E"
#define TEAM2_BEATER1_UNICODE "\u278F"
#define TEAM2_BEATER2_UNICODE "\u2790"
#define GOLDENSNITCH_UNICODE "\u2A00"
#define BLUDGER1_UNICODE "\u2A01"
#define BLUDGER2_UNICODE "\u2A01"
#define QUAFFLE_UNICODE "\u2B24"




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
};

#endif
