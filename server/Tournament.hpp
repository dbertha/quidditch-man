#ifndef TOURNAMENT_HPP
#define TOURNAMENT_HPP

#include "User.hpp"
#include <vector>

//rem : l'hôte du match est celui à gauche dans l'arbre
//rem : actuellement, un seul tournoi à la fois
//TODO : enregistrer level dans match

class Tournament {
private :
    int startingPrice;
    int startingNbOfPlayers; // must be a power of two
    int currentNbOfPlayers; //si pas encore démarré : nombre d'inscrits, si démarré : nb de joueurs restants
    int nbOfLevels; //log(startingNbOfPlayers)
    bool started;
    std::vector<std::vector<User *>> tournamentTree;
public :
    Tournament(int nbOfParticipants, int startingPrice);
    int subscribeManager(User * participant); //return -1 if already in list, 0 if not full, 1 if full
    int recordResult(User * winner, int level); //return -1 if already in list, 0 if not full, 1 if full (next level should start)
    std::vector<User *> getNextMatches(); //return a power of two number of the players for the next level
    
    int getReward(int level);
    int getStartingPrice();
    int getStartingNbOfPlayers();
    int getCurrentNbOfPlayers();
};

#endif
