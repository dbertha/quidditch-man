#ifndef MATCH_HPP
#define MATCH_HPP

#include "../common/HexagonalField.hpp"
#include "../common/Defines.hpp"
#include "ManagedPlayer.hpp"
#include "PlayingPlayer.hpp"
#include "Ball.hpp"
#include <vector>
#include <iostream>
#include <cstring> //memcpy

//compilation du module match avec tests :
//g++ -Wall -Wextra  testMatch.cpp Match.cpp PlayingPlayer.cpp HexagonalField.cpp ManagedPlayer.cpp Player.cpp Broomstick.cpp -o match.out




//TODO : logger ce qu'il se passe pour le renvoyer au client
//TODO : réunit tous les define propres au match

class Match {
    //ne connait pas le réseau, la partie réseau est gérée par matchHandler
private :
    std::vector <PlayingPlayer> __players;
    std::vector <Ball> __balls;
public : //test purpose only
    HexagonalField __field;
private :
    int __winner; //0:no winner, 1:team1, 2:team2
    std::vector<int> __indexesSortedBySpeed;
    int __scoreTeam1;
    int __scoreTeam2;
public :
    Match(std::vector<ManagedPlayer> &team1); //match en attente de recevoir l'équipe adverse
    void launch(std::vector<ManagedPlayer> &team2);
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
    bool isInVector(std::vector<int> toTest, int value);
    int getScoreTeam1(){return __scoreTeam1;}
    int getScoreTeam2(){return __scoreTeam2;}
    void serializeScoreAndPositions(char * bufferPosition);
};

#endif
