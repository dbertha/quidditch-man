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
#include "DataBase.hpp"


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
    int **__movesTeam1;
    int **__movesTeam2;
    bool __isInTournament;
public :
    Match(std::vector<ManagedPlayer> &team1, int ** movesTeam1, bool tournament = false); //match en attente de recevoir l'équipe adverse
    //~ Match();
    //~ void addTeam(std::vector<ManagedPlayer> &team, int ** movesTeam, int numTeam);
    void launch(std::vector<ManagedPlayer> &team2, int ** movesTeam2);
    void launchTrainingMatch();
    Match(std::vector<ManagedPlayer> &team1, std::vector<ManagedPlayer> &team2, int ** movesTeam1, int **movesTeam2);
    //Listes ordonnées de 7 joueurs telles que 
    //~ KEEPER en position 0
    //~ SEEKER 1
    //~ CHASER1 2
    //~ CHASER2 3
    //~ CHASER3 4
    //~ BEATER1 5
    //~ BEATER2 6
    void sortBySpeed(); //update __indexesSortedBySpeed
    void makeMoves();
    int getWinner();
    PlayingPlayer * getPlayer(int indexObject);
    bool isInVector(std::vector<int> toTest, int value);
    int getScoreTeam1(){return __scoreTeam1;}
    int getScoreTeam2(){return __scoreTeam2;}
    int serializeScoreAndPositions(char * bufferPosition); //retourne aussi le gagnant
    void serializePlayerAttr(int playerID, char * bufferPosition);
    bool isInTournament(){return __isInTournament;}
    std::vector<int> getLifesOfTeam(int nbTeam);
    void deleteIATeam();
    void generateIAActions();
};

#endif
