#ifndef MATCH_HPP
#define MATCH_HPP

#include "HexagonalField.hpp"
#include "ManagedPlayer.hpp"
#include "PlayingPlayer.hpp"
#include "Ball.hpp"
#include <vector>

//TODO : définir les positions de départ en fonction de la taille du terrain

#define GOAL_SIDE1_DIAG 9
#define GOAL_SIDE1_LINE 0
#define GOAL_SIDE2_DIAG -9
#define GOAL_SIDE2_LINE 0

#define STARTINGDIAG_TEAM1_KEEPER -8
#define STARTINGLINE_TEAM1_KEEPER 0

#define STARTINGDIAG_TEAM1_SEEKER -9
#define STARTINGLINE_TEAM1_SEEKER 9

#define STARTINGDIAG_TEAM1_CHASER1 0
#define STARTINGLINE_TEAM1_CHASER1 -6

#define STARTINGDIAG_TEAM1_CHASER2 -3
#define STARTINGLINE_TEAM1_CHASER2 0

#define STARTINGDIAG_TEAM1_CHASER3 -6
#define STARTINGLINE_TEAM1_CHASER3 6

#define STARTINGDIAG_TEAM1_BEATER1 1
#define STARTINGLINE_TEAM1_BEATER1  -5

#define STARTINGDIAG_TEAM1_BEATER2 -4
#define STARTINGLINE_TEAM1_BEATER2 5

#define STARTINGDIAG_TEAM2_KEEPER 8
#define STARTINGLINE_TEAM2_KEEPER 0

#define STARTINGDIAG_TEAM2_SEEKER 9
#define STARTINGLINE_TEAM2_SEEKER -9
 
#define STARTINGDIAG_TEAM2_CHASER1 6
#define STARTINGLINE_TEAM2_CHASER1 -6

#define STARTINGDIAG_TEAM2_CHASER2 3
#define STARTINGLINE_TEAM2_CHASER2 0

#define STARTINGDIAG_TEAM2_CHASER3 0
#define STARTINGLINE_TEAM2_CHASER3 6

#define STARTINGDIAG_TEAM2_BEATER1 4
#define STARTINGLINE_TEAM2_BEATER1 -5

#define STARTINGDIAG_TEAM2_BEATER2 -1
#define STARTINGLINE_TEAM2_BEATER2 5

class Match {
    //ne connait pas le réseau, la partie réseau est gérée par matchHandler
private :
    std::vector <PlayingPlayer> __players;
    std::vector <Ball> __balls;
    HexagonalField __field;
    int __winner; //0:no winner, 1:team1, 2:team2
public :
    Match(std::vector<ManagedPlayer> &team1, std::vector<ManagedPlayer> &team2);
    //Listes ordonnées de 7 joueurs telles que 
    //~ KEEPER en position 0
    //~ SEEKER 1
    //~ CHASER1 2
    //~ CHASER2 3
    //~ CHASER3 4
    //~ BEATER1 5
    //~ BEATER2 6
    void makeMoves(int movesTeam1[][4], int movesTeam2[][4]);
    int getWinner();
    PlayingPlayer * getPlayer(int indexObject);
};

#endif
