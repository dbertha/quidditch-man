#include "MatchesHandler.hpp"

void MatchesHandler::proposeForMatch(User * invitor, User * invited, std::vector<ManagedPlayer> &team1){
    if((invitor->getState() == FREE) and (invited->getState() == FREE)){
        int emptyMoves[7][4];
        invitor->setState(INVITING);
        invited->setState(INVITED);
        invitors.push_back(invitor);
        inviteds.push_back(invited);
        statesOfMatches.push_back(WAITINGACCEPTMATCH);
        matchesVector.push_back(new Match(team1)); //oblige à un delete lors de la suppression d'un match
        movesLists.push_back(emptyMoves); //2 listes de mouvements par match, vides au départ
        movesLists.push_back(emptyMoves);
    }else{
        //code to invitor: INVITATION_NOT_POSSIBLE
    }
}



void MatchesHandler::respondToMatchProposal(User * invitor, User * invited, std::vector<ManagedPlayer> &team2){
    matchIndex = std::find(invitors.begin(), invitors.end(), invitor) - invitors.begin(); 
}
