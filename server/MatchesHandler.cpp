#include "MatchesHandler.hpp"


void MatchesHandler::proposeForMatch(User * invitor, User * invited, std::vector<ManagedPlayer> &team1, int **movesTeam1){
    //si invited non null, l'invité existe bien

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

void MatchesHandler::sendInvitation(User * invitor, User * invited){
	
	
	int IDInvitor = invitor->getUserID();
	std::string nameInvitor = invitor->getUserName();
	invited->sendInvitation(IDInvitor, nameInvitor);
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
    if(statesOfMatches[matchIndex] != TRAININGMATCH){
        sendEndOfMatch(winner, FORFEIT);
#ifdef __DEBUG
    std::cout << "Forfait, le gagnant est prévenu" << std::endl;
#endif
    }
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
    if(statesOfMatches[matchIndex] != TRAININGMATCH){
        sendEndOfMatch(receiver, ASKFORDRAW);
    }else{
        sendEndOfMatch(demander, DRAWDENIED); //IA refuse toujours match nul
    }
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
	return receiver->sendEndOfMatch(code);
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
	return client->sendMatchConfirmation(answerCode);
}


void MatchesHandler::getScoresAndPositions(User * demander, char * position){
    int winner;
    int matchIndex = std::find(inviteds.begin(), inviteds.end(), demander) - inviteds.begin();
    if(matchIndex > int(inviteds.size())-1){
        matchIndex = std::find(invitors.begin(), invitors.end(), demander) - invitors.begin(); 
    }
#ifdef __DEBUG
    std::cout << "Demande des scores et des positions, index du match concerné : " << matchIndex << std::endl;
#endif
    winner = matchesVector[matchIndex]->serializeScoreAndPositions(position);
    demander->sendAnswer(); //on force l'envoi immédiat car un push pourrait immédiatement suivre
    if(winner != 0){ //si match terminé
#ifdef __DEBUG
    std::cout << "Match terminé ! " << std::endl;
#endif
        if((statesOfMatches[matchIndex] == OVER) or (statesOfMatches[matchIndex] == TRAININGMATCH)){ //si autre équipe a déjà vérifié ou un seul joueur
#ifdef __DEBUG
    std::cout << "Tous les joueurs concernés ont reçu le résultat, on peut supprimer le match" << std::endl;
#endif
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
    if(statesOfMatches[matchIndex] != TRAININGMATCH){
        std::cout << "Nom du gagnant : " << winningUser->getUserName() << std::endl;
    }
#endif
    int tournamentPrice = 0;
    if(matchesVector[matchIndex]->isInTournament()){
        tournamentPrice = __tournament->getReward();
    }
    if(statesOfMatches[matchIndex] != TRAININGMATCH){
        inviteds[matchIndex]->handleEndOfMatch(2, winnerTeam, tournamentPrice, matchesVector[matchIndex]->getLifesOfTeam(2)); //numteam, winningTeam
        invitors[matchIndex]->handleEndOfMatch(1, winnerTeam, tournamentPrice, matchesVector[matchIndex]->getLifesOfTeam(1)); //numteam, winningTeam
    }
    else{
        invitors[matchIndex]->handleEndOfTrainingMatch(1, winnerTeam, matchesVector[matchIndex]->getLifesOfTeam(1));
    }
    
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
    

void MatchesHandler::getPlayerInfos(User * demander, int playerID, char * position){
    int matchIndex = std::find(inviteds.begin(), inviteds.end(), demander) - inviteds.begin();
    if(matchIndex > int(inviteds.size())-1){
        matchIndex = std::find(invitors.begin(), invitors.end(), demander) - invitors.begin(); 
    }
#ifdef __DEBUG
    std::cout << "Demande des informations d'un joueur, index du match concerné : " << matchIndex << std::endl;
#endif    
    matchesVector[matchIndex]->serializePlayerAttr(playerID, position);
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
        }else if(statesOfMatches[matchIndex] == TRAININGMATCH){
            matchesVector[matchIndex]->generateIAActions();
            matchesVector[matchIndex]->makeMoves();
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
    if(statesOfMatches[index] == TRAININGMATCH){
        matchesVector[index]->deleteIATeam();
    }
    delete matchesVector[index];
    matchesVector[index] = NULL;
    matchesVector.erase(matchesVector.begin() + index);
    invitors[index]->state_ = FREE;
    invitors.erase(invitors.begin() + index);
    if(statesOfMatches[index] != TRAININGMATCH){
        inviteds[index]->state_ = FREE;
    }
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


int MatchesHandler::addPlayerToTournament(User * subscriber, char * answerPosition){
    int result = __tournament->subscribeManager(subscriber);
    int answerResult = result + 1; //adaptation pour le client
    memcpy(answerPosition, &answerResult, sizeof(answerResult));
    subscriber->sendAnswer();
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
	int result;
    //first player oppose second player :
	int IDOpponent = secondPlayer->getUserID();
	std::string nameOpponent = secondPlayer->getUserName();
    result = firstPlayer->inviteForTournamentMatch(IDOpponent, nameOpponent);
    
    //second player oppose first player :
    IDOpponent = firstPlayer->getUserID();
    nameOpponent = firstPlayer->getUserName();

    
    result = result and secondPlayer->inviteForTournamentMatch(IDOpponent, nameOpponent);
    return result;
}

void MatchesHandler::playTrainingMatch(User * invitor, std::vector<ManagedPlayer> &team1, int **movesTeam1){
    if(invitor->state_ == FREE){
        invitors.push_back(invitor);
        inviteds.push_back(NULL);
        statesOfMatches.push_back(TRAININGMATCH);
        matchesVector.push_back(new Match(team1, movesTeam1));
        matchesVector[matchesVector.size() - 1]->launchTrainingMatch();
        
        invitor->state_ = MATCH_INGAME;
        sendConfirmationTo(invitor, 1);
    }else{
        sendConfirmationTo(invitor, -1);
    }
}
