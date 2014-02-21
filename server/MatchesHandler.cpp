#include "MatchesHandler.hpp"

void MatchesHandler::proposeForMatch(User * invitor, User * invited, std::vector<ManagedPlayer> &team1){
    //TODO : tester si invited non null
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
        invitor->_state = MATCH_INVITING;
        invited->_state = MATCH_INVITED;
    }else{
        SerializedObject answer;
        answer.typeOfInfos = MATCH_CONFIRM;
        char * answerPosition = answer.stringData;
        int confirmation = INVITATION_NOT_POSSIBLE;
        memcpy(answerPosition, &confirmation, sizeof(confirmation));
        sendOnSocket(sockfd_, answer); //TODO : tester valeur retour
        //code to invitor: INVITATION_NOT_POSSIBLE
    }
}

bool isInvited(User * user){
    userIndex = std::find(inviteds.begin(), inviteds.end(), user) - inviteds.begin(); 
    return userIndex < inviteds.size();
}

void MatchesHandler::respondToMatchProposal(User * invited, std::vector<ManagedPlayer> &team2){
    matchIndex = std::find(invitors.begin(), invitors.end(), invitor) - invitors.begin(); 
}
