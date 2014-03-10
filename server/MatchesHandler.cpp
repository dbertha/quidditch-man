#include "MatchesHandler.hpp"

//TODO : gérer un match terminé : actuellement un client qui rejoue un match sera tjs attaché au match précédent

void MatchesHandler::proposeForMatch(User * invitor, User * invited, std::vector<ManagedPlayer> &team1, int **movesTeam1){
    //TODO : tester si invited non null
#ifdef __DEBUG
    std::cout << "Invitation à un match " << std::endl;
#endif
    if((invitor->state_ == FREE) and (invited->state_ == FREE)){
#ifdef __DEBUG
        std::cout << "Tous les deux disponibles " << std::endl;
#endif
        invitors.push_back(invitor);
        inviteds.push_back(invited);
        statesOfMatches.push_back(WAITINGACCEPTMATCH);
#ifdef __DEBUG
        std::cout << "On va initialiser le match" << std::endl;
#endif
        matchesVector.push_back(new Match(team1, movesTeam1)); //oblige à un delete lors de la suppression d'un match
#ifdef __DEBUG
        std::cout << "On change les status" << std::endl;
#endif
        invitor->state_ = MATCH_INVITING;
        invited->state_ = MATCH_INVITED;
        sendInvitation(invitor, invited);
    }else{
        sendConfirmationTo(invitor, INVITATION_NOT_POSSIBLE);
    }
#ifdef __DEBUG
    std::cout << "Fin invitation à un match " << std::endl;
#endif
}

int MatchesHandler::sendInvitation(User * invitor, User * invited){
	SerializedObject toSend;
	toSend.typeOfInfos = MATCH_INVITATION; //paquet header
	char * position = toSend.stringData;
	
	int IDInvitor = invitor->getUserID();
	std::string nameInvitor = invitor->getUserName();
	char name[USERNAME_LENGTH];
	strcpy(name, nameInvitor.c_str());
    memcpy(position, &IDInvitor, sizeof(IDInvitor));
    position += sizeof(IDInvitor);
    memcpy(position, &name, sizeof(name));
    return sendOnSocket(invited->getSockfd(), toSend);
}

void MatchesHandler::forfeit(User * demander){
    
    int matchIndex = std::find(inviteds.begin(), inviteds.end(), demander) - inviteds.begin();
    if(matchIndex > int(inviteds.size())-1){ 
        //il s'agit de l'hôte, soit de l'équipe 1
        matchIndex = std::find(invitors.begin(), invitors.end(), demander) - invitors.begin(); 
        if(statesOfMatches[matchIndex] == WAITINGFIRSTMOVE){ // mouvements de l'équipe 2 déjà reçu
            statesOfMatches[matchIndex] = OVER;
            sendConfirmationTo(inviteds[matchIndex], MOVES_CONFIRM); //on envoie la confirmation à l'équipe en attente
            sendEndOfMatch(inviteds[matchIndex], FORFEIT);
        }else{ //équipe 2 n'a pas encore envoyé ses mouvements
            sendEndOfMatch(inviteds[matchIndex], FORFEIT);
        }
    }else{ //il s'agit de l'invité, soit de l'équipe 2
        if(statesOfMatches[matchIndex] == WAITINGSECONDMOVE){ // mouvements de l'équipe 1 déjà reçu
            sendConfirmationTo(invitors[matchIndex], MOVES_CONFIRM);//on envoie la confirmation à l'équipe en attente
            sendEndOfMatch(invitors[matchIndex], FORFEIT);
        }else{ //équipe 1 n'a pas encore envoyé ses mouvements
            sendEndOfMatch(invitors[matchIndex], FORFEIT);
        }
    }
    deleteMatch(matchIndex);
}

void MatchesHandler::transmitDrawRequest(User * demander){
    int result;
    User * receiver;
    int matchIndex = std::find(inviteds.begin(), inviteds.end(), demander) - inviteds.begin();
    if(matchIndex > int(inviteds.size())-1){ 
        //il s'agit de l'hôte, soit de l'équipe 1
        matchIndex = std::find(invitors.begin(), invitors.end(), demander) - invitors.begin(); 
        receiver = inviteds[matchIndex];
        if(statesOfMatches[matchIndex] == WAITINGFIRSTMOVE){ // mouvements de l'équipe 2 déjà reçu
            statesOfMatches[matchIndex] = OVER;
            sendConfirmationTo(inviteds[matchIndex], MOVES_CONFIRM); //on envoie la confirmation à l'équipe en attente
            sendEndOfMatch(inviteds[matchIndex], ASKFORDRAW);
        }else{ //équipe 2 n'a pas encore envoyé ses mouvements
            sendEndOfMatch(inviteds[matchIndex], ASKFORDRAW);
        }
    }else{ //il s'agit de l'invité, soit de l'équipe 2
        receiver = invitors[matchIndex];
        if(statesOfMatches[matchIndex] == WAITINGSECONDMOVE){ // mouvements de l'équipe 1 déjà reçu
            sendConfirmationTo(invitors[matchIndex], MOVES_CONFIRM);//on envoie la confirmation à l'équipe en attente
            sendEndOfMatch(invitors[matchIndex], ASKFORDRAW);
        }else{ //équipe 1 n'a pas encore envoyé ses mouvements
            sendEndOfMatch(invitors[matchIndex], ASKFORDRAW);
        }
    }
}

void MatchesHandler::confirmDraw(User * responder, int confirmation){
    //confirmation : DRAWACCEPTED or DRAWDENIED
    User * receiver;
    int matchIndex = std::find(inviteds.begin(), inviteds.end(), responder) - inviteds.begin();
    if(matchIndex > int(inviteds.size())-1){ 
        //il s'agit de l'hôte, soit de l'équipe 1
        matchIndex = std::find(invitors.begin(), invitors.end(), responder) - invitors.begin(); 
        receiver = inviteds[matchIndex];
    }else{
        receiver = invitors[matchIndex];
    }
    sendEndOfMatch(receiver, confirmation);
    if(confirmation == DRAWACCEPTED){
        statesOfMatches[matchIndex] = OVER;
    }else{
        deleteMatch(matchIndex);
    }
}

int MatchesHandler::sendEndOfMatch(User * receiver, int code){
    SerializedObject toSend;
	char * position = toSend.stringData;
    if(code == FORFEIT){
        toSend.typeOfInfos = OPPONENTFORFEIT;
    }
    else if(code == ASKFORDRAW){
        toSend.typeOfInfos = OPPONENTASKFORDRAW;
    }
    else{
        toSend.typeOfInfos = DRAW_CONFIRM;
        memcpy(position, &code, sizeof(code)); //confirmation : DRAWACCEPTED or DRAWDENIED
    }
    return sendOnSocket(receiver->getSockfd(), toSend);
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
        deleteMatch(matchIndex);
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
    int winner;
    int matchIndex = std::find(inviteds.begin(), inviteds.end(), demander) - inviteds.begin();
    if(matchIndex > int(inviteds.size())-1){
        matchIndex = std::find(invitors.begin(), invitors.end(), demander) - invitors.begin(); 
    }
#ifdef __DEBUG
    std::cout << "Demande des scores et des positions, index du match concerné : " << matchIndex << std::endl;
#endif
    SerializedObject answer;
    answer.typeOfInfos = POSITIONS;
    winner = matchesVector[matchIndex]->serializeScoreAndPositions(answer.stringData);
    if(winner != 0){ //si match terminé
        if(statesOfMatches[matchIndex] == OVER){ //si autre équipe a déjà vérifié
            inviteds[matchIndex]->handleEndOfMatch(2, winner); //numteam, winningTeam
            invitors[matchIndex]->handleEndOfMatch(1, winner); //numteam, winningTeam
            deleteMatch(matchIndex);
        }else{
            statesOfMatches[matchIndex] = OVER;
        }
    }
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
            matchesVector[matchIndex]->makeMoves();
            sendConfirmationTo(inviteds[matchIndex], MOVES_CONFIRM);
            sendConfirmationTo(invitors[matchIndex], MOVES_CONFIRM);
        }else{
            statesOfMatches[matchIndex] = WAITINGSECONDMOVE;
        }
    }else{ //il s'agit de l'invité, soit de l'équipe 2
        if(statesOfMatches[matchIndex] == WAITINGSECONDMOVE){ // mouvements de l'équipe 1 déjà reçu
            statesOfMatches[matchIndex] = WAITING_MOVES;
            matchesVector[matchIndex]->makeMoves();
            sendConfirmationTo(inviteds[matchIndex], MOVES_CONFIRM);
            sendConfirmationTo(invitors[matchIndex], MOVES_CONFIRM);
        }else{
            statesOfMatches[matchIndex] = WAITINGFIRSTMOVE;
        }
    }
}

void MatchesHandler::deleteMatch(int index){
#ifdef __DEBUG
    std::cout << "Suppression du match à l'index " << index << std::endl;
#endif
    delete matchesVector[index];
    matchesVector.erase(matchesVector.begin() + index);
    invitors[index]->state_ = FREE;
    invitors.erase(invitors.begin() + index);
    inviteds[index]->state_ = FREE;
    inviteds.erase(inviteds.begin() + index);
    statesOfMatches.erase(statesOfMatches.begin() + index);
#ifdef __DEBUG
    std::cout << "Fin suppression du match " << std::endl;
#endif
}
