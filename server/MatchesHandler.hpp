#ifndef MATCHESHANDLER_HPP
#define MATCHESHANDLER_HPP

//#include "../common/HexagonalField.hpp"
#include "../common/Defines.hpp"
#include "ManagedPlayer.hpp"
//#include "PlayingPlayer.hpp"
//#include "Ball.hpp"
#include "Match.hpp"
#include "User.hpp"
#include "Tournament.hpp" //the only one to know this class
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring> //memcpy
#include <string>


//quand lance un match, marque l'utilisateur comme occupé
//quand reçoit une demande d'invitation, vérifie si l'invité n'est pas occupé

//~ MatchHandler :
//~ doit gérer les messages 
//~ #define PROPOSEMATCH 7 -> #define MATCH_CONFIRM 8
//~ enregistre une demande de match entre user1 et user2, ne répond pas MATCH_CONFIRM tant que l'autre n'a pas répondu (le client va être mis en pause sur un receive)
//~ #define ACCEPTMATCH 9 -> #define MATCH_CONFIRM 8
//~ si demande acceptée, lance un match et retient que ce match lie ces 2 users
//~ #define GETSCORESANDPOSITIONS+SCORES 19 -> #define SCORESANDPOSITIONS 20
//~ récupère une liste de positions ordonnées de l'objet match et la renvoie (appel une méthode de match)
//~ #define SELECTPLAYER 21 -> #define PLAYERINFOS 22
//~ recupère les infos du joueurs (méthodes de match) et les renvoient
//~ #define MAKEMOVESORASKENDOFMATCH 23 -> #define MOVES_CONFIRM 24
//~ si as reçu 2 listes de mouvements pour le même match, les passe au match, sinon il attend la 2ème liste (le code client doit gérer le time-out)
//~ regarde si l'adversaire a proposé un match null
//~ ACCEPTENDOFMATCH -> confirm
//~ arrête le match
//~ #define IS_MATCH_WAITING 31 -> #define MATCH_WAITING 32
//~ regarde si le user a été invité
//~ #define ASKFORENDOFMATCH 36 -> CONFIRM
//~ set un attribut du user adverse pour le marquer comme devant répondre à une demande de match null

#define WAITINGACCEPTMATCH 0
#define WAITINGFIRSTMOVE 1
#define WAITINGSECONDMOVE 2 
#define INVITORSASKENDOFMATCH 3 
#define INVITEDASKENDOFMATCH 4
#define WAITING_MOVES 5
#define OVER 6
#define WAITINGTWOPLAYERS 7
#define WAITINGSECONDPLAYER 8
#define TRAININGMATCH 9

class User;
class MatchesHandler{
private :
    std::vector<Match *> matchesVector;
    std::vector<User *> invitors;
    std::vector<User *> inviteds;
    std::vector<int> statesOfMatches; //WAITINGACCEPTMATCH, WAITINGFIRSTMOVE, WAITINGSECONDMOVE, INVITORSASKENDOFMATCH, INVITEDASKENDOFMATCH
    //std::vector<int**> movesLists; //pour chaque match, enregistre les déplacements des deux joueurs avant de les passer à l'instance de Match
    //enregistrement dans user de l'équipe
    Tournament * __tournament; //gère l'ordonnancement des matchs dans le cas d'un tournoi
public :
    MatchesHandler() : __tournament(NULL) {} //construction par défaut des listes
    void proposeForMatch(User * invitor, User * invited, std::vector<ManagedPlayer> &team1, int **movesTeam1);
    void respondToMatchProposal(User * invited, std::vector<ManagedPlayer> &team2, int **movesTeam2);
    void respondToTournamentMatch(User * responder, std::vector<ManagedPlayer> &team, int **movesTeam);
    void playTrainingMatch(User * invitor, std::vector<ManagedPlayer> &team1, int **movesTeam1);
    bool isInvited(User * user);
    int sendConfirmationTo(User * client, int answerCode);
    //~ int sendConfirmationTo(User * client, int answerCode, int numTeam);
    void getScoresAndPositions(User * demander, char * position);
    void getPlayerInfos(User * demander, int playerID, char * position);
    void recordMoves(User * demander);
    void sendInvitation(User * invitor, User * invited);
    void forfeit(User * demander);
    int sendEndOfMatch(User * receiver, int code);
    void transmitDrawRequest(User * demander);
    void confirmDraw(User * responder, int confirmation);
    void handleEndOfMatch(int winnerTeam, int matchIndex);
    void handleEndOfMatch(User * winner, int winnerTeam, int matchIndex);
    void deleteMatch(int index);
    
    int createTournament(int nbOfParticipants, int startingPrice);
    int serializeTournaments(char * buffer);
    int addPlayerToTournament(User * subscriber, char * answerPosition);
    void launchNextTournamentTurn();
    int inviteForTournamentMatch(User * firstPlayer, User * secondPlayer);
    void handleEndOfTournamentMatch(User * winningUser);
};

#endif
