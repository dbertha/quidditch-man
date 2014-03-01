#include "MatchesHandler.hpp"

void MatchesHandler::proposeForMatch(User * invitor, User * invited, std::vector<ManagedPlayer> &team1, int **movesTeam1){
    //TODO : tester si invited non null
    if((invitor->state_ == FREE) and (invited->state_ == FREE)){
        //int **emptyMoves = NULL;
        invitors.push_back(invitor);
        inviteds.push_back(invited);
        statesOfMatches.push_back(WAITINGACCEPTMATCH);
        matchesVector.push_back(new Match(team1, movesTeam1)); //oblige à un delete lors de la suppression d'un match (TODO)
        //movesLists.push_back(movesTeam1); //2 listes de mouvements par match, on enregistre déjà le pointeur vers les déplacements
        //movesLists.push_back(emptyMoves);
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

void MatchesHandler::respondToMatchProposal(User * invited, std::vector<ManagedPlayer> &team2, int **movesTeam2){
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
        //movesLists[(matchIndex*2) + 1] = movesTeam2; //on enregistre déjà le pointeur vers les déplacements
        matchesVector[matchIndex]->launch(team2, movesTeam2);
        sendConfirmationTo(invited, MATCH_STARTING);
        sendConfirmationTo(invitors[matchIndex], MATCH_STARTING);
#ifdef __DEBUG
        std::cout << "Confirmations sent " << std::endl;
#endif
        statesOfMatches[matchIndex] = WAITING_MOVES;
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

void MatchesHandler::getScoresAndPositions(User * demander){
    int matchIndex = std::find(inviteds.begin(), inviteds.end(), demander) - inviteds.begin();
    if(matchIndex > int(inviteds.size())-1){
        matchIndex = std::find(invitors.begin(), invitors.end(), demander) - invitors.begin(); 
    }
#ifdef __DEBUG
    std::cout << "Demande des scores et des positions, index du match concerné : " << matchIndex << std::endl;
#endif
    SerializedObject answer;
    answer.typeOfInfos = POSITIONS;
    matchesVector[matchIndex]->serializeScoreAndPositions(answer.stringData);
    sendOnSocket(demander->getSockfd(), answer);
}

void MatchesHandler::getPlayerInfos(User * demander, int playerID){
    int matchIndex = std::find(inviteds.begin(), inviteds.end(), demander) - inviteds.begin();
    if(matchIndex > int(inviteds.size())-1){
        matchIndex = std::find(invitors.begin(), invitors.end(), demander) - invitors.begin(); 
    }
#ifdef __DEBUG
    std::cout << "Demande des informations d'un joueur, index du match concerné : " << matchIndex << std::endl;
#endif
    SerializedObject answer;
    answer.typeOfInfos = PLAYERINFOS;
    matchesVector[matchIndex]->serializePlayerAttr(playerID, answer.stringData);
    sendOnSocket(demander->getSockfd(), answer);
}

void MatchesHandler::recordMoves(User * demander){
    int matchIndex = std::find(inviteds.begin(), inviteds.end(), demander) - inviteds.begin();
    if(matchIndex > int(inviteds.size())-1){ 
        //il s'agit de l'hôte, soit de l'équipe 1
        matchIndex = std::find(invitors.begin(), invitors.end(), demander) - invitors.begin(); 
        if(statesOfMatches[matchIndex] == WAITINGFIRSTMOVE){ // mouvements de l'équipe 2 déjà reçu
            statesOfMatches[matchIndex] = WAITING_MOVES;
            //TODO : comme il s'agit toujours du même pointeur, enregistrer les pointeurs dans l'objet match
            matchesVector[matchIndex]->makeMoves();
            sendConfirmationTo(inviteds[matchIndex], MOVES_CONFIRM);
            sendConfirmationTo(invitors[matchIndex], MOVES_CONFIRM);
        }else{
            statesOfMatches[matchIndex] = WAITINGSECONDMOVE;
        }
    }else{ //il s'agit de l'invité, soit de l'équipe 2
        if(statesOfMatches[matchIndex] == WAITINGSECONDMOVE){ // mouvements de l'équipe 1 déjà reçu
            statesOfMatches[matchIndex] = WAITING_MOVES;
            //TODO : comme il s'agit toujours du même pointeur, enregistrer les pointeurs dans l'objet match
            matchesVector[matchIndex]->makeMoves();
            sendConfirmationTo(inviteds[matchIndex], MOVES_CONFIRM);
            sendConfirmationTo(invitors[matchIndex], MOVES_CONFIRM);
        }else{
            statesOfMatches[matchIndex] = WAITINGFIRSTMOVE;
        }
    }
}