#include "MatchesHandler.hpp"

void MatchesHandler::proposeForMatch(User * invitor, User * invited, std::vector<ManagedPlayer> &team1){
    //TODO : tester si invited non null
    if((invitor->state_ == FREE) and (invited->state_ == FREE)){
        int **emptyMoves = NULL;
        invitors.push_back(invitor);
        inviteds.push_back(invited);
        statesOfMatches.push_back(WAITINGACCEPTMATCH);
        matchesVector.push_back(new Match(team1)); //oblige à un delete lors de la suppression d'un match
        movesLists.push_back(emptyMoves); //2 listes de mouvements par match, vides au départ
        movesLists.push_back(emptyMoves);
        invitor->state_ = MATCH_INVITING;
        invited->state_ = MATCH_INVITED;
    }else{
        SerializedObject answer;
        answer.typeOfInfos = MATCH_CONFIRM;
        char * answerPosition = answer.stringData;
        int confirmation = INVITATION_NOT_POSSIBLE;
        memcpy(answerPosition, &confirmation, sizeof(confirmation));
        sendOnSocket(invitor->getSockfd(), answer); //TODO : tester valeur retour
        //code to invitor: INVITATION_NOT_POSSIBLE
    }
}

bool MatchesHandler::isInvited(User * user){
    return user->state_ == MATCH_INVITED;
}

void MatchesHandler::respondToMatchProposal(User * invited, std::vector<ManagedPlayer> &team2){
    int matchIndex = std::find(invitors.begin(), invitors.end(), invited) - invitors.begin(); 
}
