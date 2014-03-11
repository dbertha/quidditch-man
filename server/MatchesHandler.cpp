#include "MatchesHandler.hpp"

//TODO : gérer un match terminé : actuellement un client qui rejoue un match sera tjs attaché au match précédent

void MatchesHandler::proposeForMatch(User * invitor, User * invited, std::vector<ManagedPlayer> &team1, int **movesTeam1){
    //TODO : tester si invited non null

    if((invitor->state_ == FREE) and (invited->state_ == FREE)){

        invitors.push_back(invitor);
        inviteds.push_back(invited);
        statesOfMatches.push_back(WAITINGACCEPTMATCH);

        matchesVector.push_back(new Match(team1, movesTeam1)); //oblige à un delete lors de la suppression d'un match

        invitor->state_ = MATCH_INVITING;
        invited->state_ = MATCH_INVITED;
        sendInvitation(invitor, invited);
    }else{
        sendConfirmationTo(invitor, INVITATION_NOT_POSSIBLE);
    }
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
    User * winner;
    int winningTeam;
    int matchIndex = std::find(inviteds.begin(), inviteds.end(), demander) - inviteds.begin();
    if(matchIndex > int(inviteds.size())-1){ 
        //il s'agit de l'hôte, soit de l'équipe 1
        matchIndex = std::find(invitors.begin(), invitors.end(), demander) - invitors.begin(); 
        winner = inviteds[matchIndex];
        winningTeam = 2;
        if(statesOfMatches[matchIndex] == WAITINGFIRSTMOVE){ // mouvements de l'équipe 2 déjà reçu
            statesOfMatches[matchIndex] = OVER;
            sendConfirmationTo(inviteds[matchIndex], MOVES_CONFIRM); //on envoie la confirmation à l'équipe en attente
            //sendEndOfMatch(inviteds[matchIndex], FORFEIT);
        }else{ //équipe 2 n'a pas encore envoyé ses mouvements
            //sendEndOfMatch(inviteds[matchIndex], FORFEIT);
        }
    }else{ //il s'agit de l'invité, soit de l'équipe 2
        winner = invitors[matchIndex];
        winningTeam = 1;
        if(statesOfMatches[matchIndex] == WAITINGSECONDMOVE){ // mouvements de l'équipe 1 déjà reçu
            sendConfirmationTo(invitors[matchIndex], MOVES_CONFIRM);//on envoie la confirmation à l'équipe en attente
            //sendEndOfMatch(invitors[matchIndex], FORFEIT);
        }else{ //équipe 1 n'a pas encore envoyé ses mouvements
            //sendEndOfMatch(invitors[matchIndex], FORFEIT);
        }
    }
    sendEndOfMatch(winner, FORFEIT);
    handleEndOfMatch(winner, winningTeam, matchIndex);
    
}

void MatchesHandler::transmitDrawRequest(User * demander){
    User * receiver;
    int matchIndex = std::find(inviteds.begin(), inviteds.end(), demander) - inviteds.begin();
    if(matchIndex > int(inviteds.size())-1){ 
        //il s'agit de l'hôte, soit de l'équipe 1
        matchIndex = std::find(invitors.begin(), invitors.end(), demander) - invitors.begin(); 
        receiver = inviteds[matchIndex];
        if(statesOfMatches[matchIndex] == WAITINGFIRSTMOVE){ // mouvements de l'équipe 2 déjà reçu
            statesOfMatches[matchIndex] = OVER;
            sendConfirmationTo(inviteds[matchIndex], MOVES_CONFIRM); //on envoie la confirmation à l'équipe en attente
            //sendEndOfMatch(inviteds[matchIndex], ASKFORDRAW);
        }else{ //équipe 2 n'a pas encore envoyé ses mouvements
            //sendEndOfMatch(inviteds[matchIndex], ASKFORDRAW);
        }
    }else{ //il s'agit de l'invité, soit de l'équipe 2
        receiver = invitors[matchIndex];
        if(statesOfMatches[matchIndex] == WAITINGSECONDMOVE){ // mouvements de l'équipe 1 déjà reçu
            sendConfirmationTo(invitors[matchIndex], MOVES_CONFIRM);//on envoie la confirmation à l'équipe en attente
            //sendEndOfMatch(invitors[matchIndex], ASKFORDRAW);
        }else{ //équipe 1 n'a pas encore envoyé ses mouvements
            //sendEndOfMatch(invitors[matchIndex], ASKFORDRAW);
        }
    }
    sendEndOfMatch(receiver, ASKFORDRAW);
}

void MatchesHandler::confirmDraw(User * responder, int confirmation){
    //confirmation : DRAWACCEPTED or DRAWDENIED
    User * receiver;
    int winningTeam = 2;
    int matchIndex = std::find(inviteds.begin(), inviteds.end(), responder) - inviteds.begin();
    if(matchIndex > int(inviteds.size())-1){ 
        //il s'agit de l'hôte, soit de l'équipe 1
        matchIndex = std::find(invitors.begin(), invitors.end(), responder) - invitors.begin(); 
        receiver = inviteds[matchIndex];
        winningTeam = 1;
    }else{
        receiver = invitors[matchIndex];
    }
    sendEndOfMatch(receiver, confirmation);
    if(confirmation == DRAWACCEPTED){
        if(matchesVector[matchIndex]->isInTournament()){
            //si tournoi, celui qui fait la demande est perdant
            handleEndOfMatch(responder, winningTeam, matchIndex);
        }else{
            deleteMatch(matchIndex);
        }
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

//~ int MatchesHandler::sendConfirmationTo(User * client, int answerCode, int numTeam){
    //~ //idem but with numTeam at the end of buffer
    //~ SerializedObject answer;
    //~ answer.typeOfInfos = MATCH_CONFIRM;
    //~ char * answerPosition = answer.stringData;
    //~ int confirmation = answerCode;
    //~ memcpy(answerPosition, &confirmation, sizeof(confirmation));
    //~ answerPosition += sizeof(confirmation));
    //~ memcpy(answerPosition, &numTeam, sizeof(numTeam));
    //~ return sendOnSocket(client->getSockfd(), answer); //TODO : tester valeur retour
//~ }

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
    sendOnSocket(demander->getSockfd(), answer);
    if(winner != 0){ //si match terminé
        if(statesOfMatches[matchIndex] == OVER){ //si autre équipe a déjà vérifié
            handleEndOfMatch(winner, matchIndex);
        }else{
            statesOfMatches[matchIndex] = OVER;
        }
    }
}

void MatchesHandler::handleEndOfMatch(int winner, int matchIndex){
    User * winningUser;
    winningUser = winner == 1 ? invitors[matchIndex] : inviteds[matchIndex];
    handleEndOfMatch(winningUser, winner, matchIndex); //surcharge
}

void MatchesHandler::handleEndOfMatch(User * winningUser, int winnerTeam, int matchIndex){
#ifdef __DEBUG
    std::cout << "Nom du gagnant : " << winningUser->getUserName() << std::endl;
#endif
    int tournamentPrice = 0;
    if(matchesVector[matchIndex]->isInTournament()){
        tournamentPrice = __tournament->getReward();
    }
    inviteds[matchIndex]->handleEndOfMatch(2, winnerTeam, tournamentPrice); //numteam, winningTeam
    invitors[matchIndex]->handleEndOfMatch(1, winnerTeam, tournamentPrice); //numteam, winningTeam
    
    if(matchesVector[matchIndex]->isInTournament()){
        deleteMatch(matchIndex);
        handleEndOfTournamentMatch(winningUser); //after deletion, recording winner and test if next level should start (and start it then)
    }else{
        deleteMatch(matchIndex);
    }
}

void MatchesHandler::handleEndOfTournamentMatch(User * winningUser){
    int result = __tournament->recordResult(winningUser);
    if(result == -1){
#ifdef __DEBUG
    std::cout << "Tournoi terminé, nom du gagnant final : " << winningUser->getUserName() << std::endl;
#endif
        //tournament is over
        delete __tournament;
        __tournament = NULL;
        //TODO : send message to the ultimate winner about his final victory
    }else if(result == 1){
        //next level is ready to start
#ifdef __DEBUG
    std::cout << "On lance le tour suivant du tournoi " << std::endl;
#endif
        launchNextTournamentTurn();
    }
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
    matchesVector[index] = NULL;
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

int MatchesHandler::createTournament(int nbOfParticipants, int startingPrice){
    if(__tournament != NULL){
        return 0; //error, only one tournament at the same time
    }
    __tournament = new Tournament(nbOfParticipants, startingPrice);
    return 1;
}

int MatchesHandler::serializeTournaments(char * buffer){
    //only one ==> size fixed
    int nbOfTournaments;
    if(not (__tournament == NULL) and not (__tournament->isStarted())){ //only if not started
        nbOfTournaments = 1;
        memcpy(buffer, &nbOfTournaments, sizeof(nbOfTournaments));
        buffer += sizeof(nbOfTournaments);
        return __tournament->serialize(buffer);
    }
    nbOfTournaments = 0;
    memcpy(buffer, &nbOfTournaments, sizeof(nbOfTournaments));
    return 1;
}


int MatchesHandler::addPlayerToTournament(User * subscriber){
    int result = __tournament->subscribeManager(subscriber);
    SerializedObject answer;
    char * answerPosition = answer.stringData;
    int answerResult = result + 1; //adaptation pour le client
    answer.typeOfInfos = JOINTOURNAMENT_CONFIRM;
    memcpy(answerPosition, &answerResult, sizeof(answerResult));
    sendOnSocket(subscriber->getSockfd(), answer); //TODO : tester retour
    if(result == 1){ //tournament starts
        launchNextTournamentTurn();
    }
    return result;
}

void MatchesHandler::launchNextTournamentTurn(){
    std::vector<User *> turnParticipants = __tournament->getNextMatches();
#ifdef __DEBUG
    std::cout << "Nombre de participants pour le prochain tour : " << turnParticipants.size() << std::endl;
#endif
    for(unsigned int i = 0; i < turnParticipants.size(); i+=2){
        inviteForTournamentMatch(turnParticipants[i], turnParticipants[i+1]);
        invitors.push_back(turnParticipants[i]);
        inviteds.push_back(turnParticipants[i+1]);
        statesOfMatches.push_back(WAITINGTWOPLAYERS);
        matchesVector.push_back(NULL); //initialisé à la réception de la première équipe
        turnParticipants[i]->state_ = MATCH_INVITED;
        turnParticipants[i+1]->state_ = MATCH_INVITED;
    }
}

void MatchesHandler::respondToTournamentMatch(User * responder, std::vector<ManagedPlayer> &team, int **movesTeam){
    int matchIndex = std::find(invitors.begin(), invitors.end(), responder) - invitors.begin(); 
    
    if(matchIndex > int(invitors.size())-1){ 
        matchIndex = std::find(inviteds.begin(), inviteds.end(), responder) - inviteds.begin();
        if(statesOfMatches[matchIndex] == WAITINGTWOPLAYERS){
            //première équipe à répondre doit être dans le vecteur invitors
            User * temp = invitors[matchIndex];
            invitors[matchIndex] = inviteds[matchIndex];
            inviteds[matchIndex] = temp;
        }
    }
         
#ifdef __DEBUG
        std::cout << "Team for match received" << std::endl;
#endif
    if(statesOfMatches[matchIndex] == WAITINGTWOPLAYERS){
        matchesVector[matchIndex] = new Match(team, movesTeam, true); //initialisation
        statesOfMatches[matchIndex] = WAITINGSECONDPLAYER;
    }else{
        matchesVector[matchIndex]->launch(team,movesTeam);
        sendConfirmationTo(invitors[matchIndex], 1); //code = numTeam
        sendConfirmationTo(inviteds[matchIndex], 2);
#ifdef __DEBUG
        std::cout << "Confirmations sent " << std::endl;
#endif
        statesOfMatches[matchIndex] = WAITING_MOVES;
    }
}

int MatchesHandler::inviteForTournamentMatch(User * firstPlayer, User * secondPlayer){
	SerializedObject msgForFirstPlayer, msgForSecondPlayer;
	msgForFirstPlayer.typeOfInfos = msgForSecondPlayer.typeOfInfos = MATCH_TOURNAMENT_START; //paquet header
    //first player oppose second player :
	char * position = msgForFirstPlayer.stringData;
	int IDOpponent = secondPlayer->getUserID();
	std::string nameOpponent = secondPlayer->getUserName();
	char name[USERNAME_LENGTH];
    int  result;
	strcpy(name, nameOpponent.c_str());
    memcpy(position, &IDOpponent, sizeof(IDOpponent));
    position += sizeof(IDOpponent);
    memcpy(position, &name, sizeof(name));
    position += sizeof(name);
    
    //second player oppose first player :
    position = msgForSecondPlayer.stringData;
    IDOpponent = firstPlayer->getUserID();
    nameOpponent = firstPlayer->getUserName();
    strcpy(name, nameOpponent.c_str());
    memcpy(position, &IDOpponent, sizeof(IDOpponent));
    position += sizeof(IDOpponent);
    memcpy(position, &name, sizeof(name));
    position += sizeof(name);

    result = sendOnSocket(firstPlayer->getSockfd(), msgForFirstPlayer);
    result = result and sendOnSocket(secondPlayer->getSockfd(), msgForSecondPlayer);
    return result;
}
