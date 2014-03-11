#ifndef TOURNAMENT_HPP
#define TOURNAMENT_HPP

#include "User.hpp"
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <cmath>
//rem : l'hôte du match est celui à gauche dans l'arbre
//rem : actuellement, un seul tournoi à la fois, un seul niveau à la fois
//TODO : gérer déconnexions
//TODO : backup par DataBase

#define REWARDGROWINGRATE 1.1

class Tournament {
private :
    int __startingPrice;
    int __startingNbOfPlayers; // must be a power of two
    int __currentNbOfPlayers; //si pas encore démarré : nombre d'inscrits, si démarré : nb de joueurs restants
    int __nbOfLevels; //log(startingNbOfPlayers), currentLevel = nbOfLevelsLeft
    bool __started;
    std::vector<std::vector<User *> > __tournamentTree;
public :
    Tournament(int nbOfParticipants, int startingPrice);
    int subscribeManager(User * participant); //return -1 if already in list, 0 if not full, 1 if full after addition
    int recordResult(User * winner); //return -1 if already in list, 0 if not full, 1 if full (next level should start)
    std::vector<User *> getNextMatches(); //return a power of two number of the players for the next level
    
    int getReward(int level);
    int getReward(); //currentlevel
    int getStartingPrice() const{ return __startingPrice;}
    int getStartingNbOfPlayers()const {return __startingNbOfPlayers;}
    int getCurrentNbOfPlayers() const {return __currentNbOfPlayers;}
    int getCurrentLevel() const {return __nbOfLevels;}
};

int log2(int x);

#endif
