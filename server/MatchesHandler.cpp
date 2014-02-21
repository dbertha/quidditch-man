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
        sendConfirmationTo(invitor, INVITATION_NOT_POSSIBLE);
        //code to invitor: INVITATION_NOT_POSSIBLE
    }
}

bool MatchesHandler::isInvited(User * user){
    return user->state_ == MATCH_INVITED;
}

void MatchesHandler::respondToMatchProposal(User * invited, std::vector<ManagedPlayer> &team2){
    int matchIndex = std::find(inviteds.begin(), inviteds.end(), invited) - inviteds.begin(); 
    if(team2.empty()){//refuse le match
#ifdef __DEBUG
        std::cout << "Match refused " << std::endl;
#endif
        sendConfirmationTo(invited, MATCH_DENIED);
        sendConfirmationTo(invitors[matchIndex], MATCH_DENIED);
        //TODO : supprimer les infos de ce match des vecteurs
    }else{
#ifdef __DEBUG
        std::cout << "Match lauched " << std::endl;
#endif
        matchesVector[matchIndex]->launch(team2);
        sendConfirmationTo(invited, MATCH_STARTING);
        sendConfirmationTo(invitors[matchIndex], MATCH_STARTING);
    }
}

int MatchesHandler::sendConfirmationTo(User * client, int answerCode){
    SerializedObject answer;
    answer.typeOfInfos = MATCH_CONFIRM;
    char * answerPosition = answer.stringData;
    int confirmation = answerCode;
    memcpy(answerPosition, &confirmation, sizeof(confirmation));
    return sendOnSocket(client->getSockfd(), answer); //TODO : tester valeur retour
}