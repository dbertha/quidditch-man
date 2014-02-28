#include "Match.hpp"


//makeMoves :
//si quaffle passe par un but, augmenter score
//si seeker(attrapeur)
//si keeper (gardien)
//si chaser (poursuiveur : quaffle)
//si beater (batteur)
//si bludger collisionne joueur, handlerBludger
Match::Match(std::vector<ManagedPlayer> &team1): __players(), __field(), __winner(0){
#ifdef __DEBUG
        std::cout << "Match généré à partir de la première équipe" << std::endl;
#endif
        for(int i = 0; i < 7; ++i){
        int role = i; //les 2 premiers roles correspondent déjà
        if((role < TEAM1_BEATER1) and (role > TEAM1_SEEKER)) {role = ROLE_CHASER;}
        else if((role < TEAM2_KEEPER) and (role > TEAM1_CHASER3)) {role = ROLE_BEATER;}
        AxialCoordinates position;
        switch(i){
            case TEAM1_KEEPER :
                position = AxialCoordinates(STARTINGDIAG_TEAM1_KEEPER, STARTINGLINE_TEAM1_KEEPER);
                break;
            case TEAM1_SEEKER :
                position = AxialCoordinates(STARTINGDIAG_TEAM1_SEEKER, STARTINGLINE_TEAM1_SEEKER);
                break;
            case TEAM1_CHASER1 :
                position = AxialCoordinates(STARTINGDIAG_TEAM1_CHASER1, STARTINGLINE_TEAM1_CHASER1);
                break;
            case TEAM1_CHASER2 :
                position = AxialCoordinates(STARTINGDIAG_TEAM1_CHASER2, STARTINGLINE_TEAM1_CHASER2);
                break;
            case TEAM1_CHASER3 :
                position = AxialCoordinates(STARTINGDIAG_TEAM1_CHASER3, STARTINGLINE_TEAM1_CHASER3);
                break;
            case TEAM1_BEATER1 :
                position = AxialCoordinates(STARTINGDIAG_TEAM1_BEATER1, STARTINGLINE_TEAM1_BEATER1);
                break;
            case TEAM1_BEATER2 :
                position = AxialCoordinates(STARTINGDIAG_TEAM1_BEATER2, STARTINGLINE_TEAM1_BEATER2);
                break;
        }
        __players.push_back(PlayingPlayer((team1[i]), role, position));
        __field.setOccupant(position, i);
    }
}

void Match::launch(std::vector<ManagedPlayer> &team2){ //suite du constructeur
#ifdef __DEBUG
        std::cout << "Match démarré avec la réception de la 2 ème équipe" << std::endl;
#endif
    for(int i = 0; i < 7; ++i){ //création et positionnement de l'équipe 2
        int role = i % TEAM2_KEEPER; //les 2 premiers roles correspondent déjà
        if((role < TEAM1_BEATER1) and (role > TEAM1_SEEKER)) {role = ROLE_CHASER;}
        else if((role < TEAM2_KEEPER) and (role > TEAM1_CHASER3)) {role = ROLE_BEATER;}
        AxialCoordinates position;
        switch(i+TEAM2_KEEPER){
            case TEAM2_KEEPER :
                position = AxialCoordinates(STARTINGDIAG_TEAM2_KEEPER, STARTINGLINE_TEAM2_KEEPER);
                break;
            case TEAM2_SEEKER :
                position = AxialCoordinates(STARTINGDIAG_TEAM2_SEEKER, STARTINGLINE_TEAM2_SEEKER);
                break;
            case TEAM2_CHASER1 :
                position = AxialCoordinates(STARTINGDIAG_TEAM2_CHASER1, STARTINGLINE_TEAM2_CHASER1);
                break;
            case TEAM2_CHASER2 :
                position = AxialCoordinates(STARTINGDIAG_TEAM2_CHASER2, STARTINGLINE_TEAM2_CHASER2);
                break;
            case TEAM2_CHASER3 :
                position = AxialCoordinates(STARTINGDIAG_TEAM2_CHASER3, STARTINGLINE_TEAM2_CHASER3);
                break;
            case TEAM2_BEATER1 :
                position = AxialCoordinates(STARTINGDIAG_TEAM2_BEATER1, STARTINGLINE_TEAM2_BEATER1);
                break;
            case TEAM2_BEATER2 :
                position = AxialCoordinates(STARTINGDIAG_TEAM2_BEATER2, STARTINGLINE_TEAM2_BEATER2);
                break;
        }
        __players.push_back(PlayingPlayer((team2[i]), role, position));
        __field.setOccupant(position, i+TEAM2_KEEPER);
    }
    //les 4 balles :
    __balls.push_back(Ball(GOLDENSNITCH, AxialCoordinates(STARTINGDIAG_GOLDENSNITCH, STARTINGLINE_GOLDENSNITCH))); 
    __field.setOccupant(AxialCoordinates(STARTINGDIAG_GOLDENSNITCH, STARTINGLINE_GOLDENSNITCH), GOLDENSNITCH);
    __balls.push_back(Ball(BLUDGER1, AxialCoordinates(STARTINGDIAG_BLUDGER1, STARTINGLINE_BLUDGER1))); 
    __field.setOccupant(AxialCoordinates(STARTINGDIAG_BLUDGER1, STARTINGLINE_BLUDGER1), BLUDGER1);
    __balls.push_back(Ball(GOLDENSNITCH, AxialCoordinates(STARTINGDIAG_BLUDGER2, STARTINGLINE_BLUDGER2))); 
    __field.setOccupant(AxialCoordinates(STARTINGDIAG_BLUDGER2, STARTINGLINE_BLUDGER2), BLUDGER2);
    __balls.push_back(Ball(GOLDENSNITCH, AxialCoordinates(STARTINGDIAG_QUAFFLE, STARTINGLINE_QUAFFLE))); 
    __field.setOccupant(AxialCoordinates(STARTINGDIAG_QUAFFLE, STARTINGLINE_QUAFFLE), QUAFFLE);
#ifdef __DEBUG
    __field.display();
    std::cout << "On passe aux priorités." << std::endl;
#endif
    
    //on enregistre les priorités de déplacements :
    //TODO : à optimiser
    int maxIndex, maxSpeedValue;
    for(unsigned int i = 0; i < 18; ++i){
        maxIndex = -1;
        maxSpeedValue = 0;
        for(unsigned int player = 0; player < __players.size(); ++player){
            if(not isInVector(__indexesSortedBySpeed, player)){
                if(__players[player].getCapacity(SPEED) > maxSpeedValue){
                    maxIndex = player;
                    maxSpeedValue = __players[player].getCapacity(SPEED);
                }
            }
        }
        std::cout << "On regarde les balles. i : " << i << std::endl;
        for(unsigned int ball = GOLDENSNITCH; ball < (GOLDENSNITCH + __balls.size()); ++ball){
            if(not isInVector(__indexesSortedBySpeed, ball)){
                if(__balls[ball - GOLDENSNITCH].getSpeed() > maxSpeedValue){
                    maxIndex = ball;
                    maxSpeedValue = __balls[ball - GOLDENSNITCH].getSpeed();
                }
            }
        }
        std::cout << "nouvel index choisi." << maxIndex << std::endl;
        __indexesSortedBySpeed.push_back(maxIndex);
    }
    std::cout << "Match constructed" << std::endl;
    
}

Match::Match(std::vector<ManagedPlayer> &team1, std::vector<ManagedPlayer> &team2) :
    __players(), __field(), __winner(0){
    
    for(int i = 0; i < 7; ++i){
        int role = i % TEAM2_KEEPER; //les 2 premiers roles correspondent déjà
        if((role < TEAM1_BEATER1) and (role > TEAM1_SEEKER)) {role = ROLE_CHASER;}
        else if((role < TEAM2_KEEPER) and (role > TEAM1_CHASER3)) {role = ROLE_BEATER;}
        AxialCoordinates position;
        switch(i){
            case TEAM1_KEEPER :
                position = AxialCoordinates(STARTINGDIAG_TEAM1_KEEPER, STARTINGLINE_TEAM1_KEEPER);
                break;
            case TEAM1_SEEKER :
                position = AxialCoordinates(STARTINGDIAG_TEAM1_SEEKER, STARTINGLINE_TEAM1_SEEKER);
                break;
            case TEAM1_CHASER1 :
                position = AxialCoordinates(STARTINGDIAG_TEAM1_CHASER1, STARTINGLINE_TEAM1_CHASER1);
                break;
            case TEAM1_CHASER2 :
                position = AxialCoordinates(STARTINGDIAG_TEAM1_CHASER2, STARTINGLINE_TEAM1_CHASER2);
                break;
            case TEAM1_CHASER3 :
                position = AxialCoordinates(STARTINGDIAG_TEAM1_CHASER3, STARTINGLINE_TEAM1_CHASER3);
                break;
            case TEAM1_BEATER1 :
                position = AxialCoordinates(STARTINGDIAG_TEAM1_BEATER1, STARTINGLINE_TEAM1_BEATER1);
                break;
            case TEAM1_BEATER2 :
                position = AxialCoordinates(STARTINGDIAG_TEAM1_BEATER2, STARTINGLINE_TEAM1_BEATER2);
                break;
        }
        __players.push_back(PlayingPlayer((team1[i]), role, position));
        __field.setOccupant(position, i);
    }
    
    for(int i = 0; i < 7; ++i){ //création et positionnement de l'équipe 2
        int role = i % TEAM2_KEEPER; //les 2 premiers roles correspondent déjà
        if((role < TEAM1_BEATER1) and (role > TEAM1_SEEKER)) {role = ROLE_CHASER;}
        else if((role < TEAM2_KEEPER) and (role > TEAM1_CHASER3)) {role = ROLE_BEATER;}
        AxialCoordinates position;
        switch(i+TEAM2_KEEPER){
            case TEAM2_KEEPER :
                position = AxialCoordinates(STARTINGDIAG_TEAM2_KEEPER, STARTINGLINE_TEAM2_KEEPER);
                break;
            case TEAM2_SEEKER :
                position = AxialCoordinates(STARTINGDIAG_TEAM2_SEEKER, STARTINGLINE_TEAM2_SEEKER);
                break;
            case TEAM2_CHASER1 :
                position = AxialCoordinates(STARTINGDIAG_TEAM2_CHASER1, STARTINGLINE_TEAM2_CHASER1);
                break;
            case TEAM2_CHASER2 :
                position = AxialCoordinates(STARTINGDIAG_TEAM2_CHASER2, STARTINGLINE_TEAM2_CHASER2);
                break;
            case TEAM2_CHASER3 :
                position = AxialCoordinates(STARTINGDIAG_TEAM2_CHASER3, STARTINGLINE_TEAM2_CHASER3);
                break;
            case TEAM2_BEATER1 :
                position = AxialCoordinates(STARTINGDIAG_TEAM2_BEATER1, STARTINGLINE_TEAM2_BEATER1);
                break;
            case TEAM2_BEATER2 :
                position = AxialCoordinates(STARTINGDIAG_TEAM2_BEATER2, STARTINGLINE_TEAM2_BEATER2);
                break;
        }
        __players.push_back(PlayingPlayer((team2[i]), role, position));
        __field.setOccupant(position, i+TEAM2_KEEPER);
    }
    //les 4 balles :
    __balls.push_back(Ball(GOLDENSNITCH, AxialCoordinates(STARTINGDIAG_GOLDENSNITCH, STARTINGLINE_GOLDENSNITCH))); 
    __field.setOccupant(AxialCoordinates(STARTINGDIAG_GOLDENSNITCH, STARTINGLINE_GOLDENSNITCH), GOLDENSNITCH);
    __balls.push_back(Ball(BLUDGER1, AxialCoordinates(STARTINGDIAG_BLUDGER1, STARTINGLINE_BLUDGER1))); 
    __field.setOccupant(AxialCoordinates(STARTINGDIAG_BLUDGER1, STARTINGLINE_BLUDGER1), BLUDGER1);
    __balls.push_back(Ball(GOLDENSNITCH, AxialCoordinates(STARTINGDIAG_BLUDGER2, STARTINGLINE_BLUDGER2))); 
    __field.setOccupant(AxialCoordinates(STARTINGDIAG_BLUDGER2, STARTINGLINE_BLUDGER2), BLUDGER2);
    __balls.push_back(Ball(GOLDENSNITCH, AxialCoordinates(STARTINGDIAG_QUAFFLE, STARTINGLINE_QUAFFLE))); 
    __field.setOccupant(AxialCoordinates(STARTINGDIAG_QUAFFLE, STARTINGLINE_QUAFFLE), QUAFFLE);
    __field.display();
    
    //on enregistre les priorités de déplacements :
    //TODO : à optimiser
    int maxIndex, maxSpeedValue;
    for(int i; i < 18; ++i){
        maxIndex = -1;
        maxSpeedValue = 0;
        for(unsigned int player = 0; player < __players.size(); ++player){
            if(not isInVector(__indexesSortedBySpeed, player)){
                if(__players[player].getCapacity(SPEED) > maxSpeedValue){
                    maxIndex = player;
                    maxSpeedValue = __players[player].getCapacity(SPEED);
                }
            }
        }
        for(unsigned int ball = GOLDENSNITCH; ball < (GOLDENSNITCH + __balls.size()); ++ball){
            if(not isInVector(__indexesSortedBySpeed, ball)){
                if(__balls[ball - GOLDENSNITCH].getSpeed() > maxSpeedValue){
                    maxIndex = ball;
                    maxSpeedValue = __balls[ball - GOLDENSNITCH].getSpeed();
                }
            }
        }
        __indexesSortedBySpeed.push_back(maxIndex);
    }
    
}

void Match::makeMoves(int movesTeam1[][4], int movesTeam2[][4]){
    //moves[][0] : indice de l'objet concerné
    //moves[][1] : action spéciale
    //moves[][2] : diagonale destination
    //moves[][3] : ligne destination
    //1ère étape : gestion des interceptions de balle
    
    //TODO : si chaser lance le souaffle, lui donner une position de départ
    for(int i = 0; i < 7; ++i){
        if(movesTeam1[i][1] >= 0){ //tentative de capture d'une balle
            if((__players[movesTeam1[i][0]].getRole() == ROLE_CHASER) and (movesTeam1[i][1] == INTERCEPT_QUAFFLE)){ //normalement c'est test est fait aussi niveau client
                //tentative de récupérer le quaffle
                //calcul de la distance entre l'attrapeur et la balle
                //TODO : meilleure gestion de l'indice des balles
                int distance = __players[movesTeam1[i][0]].getPosition().getDistanceTo(__balls[3].getPosition());
                if(__players[movesTeam1[i][0]].interceptQuaffle(distance)){ //si attrapage réussi
                    __players[movesTeam1[i][0]].carryQuaffle(); //le joueur prend la balle avec lui
                    __field.setOccupant(__balls[3].getPosition(), -1); //balle sort du terrain
                    __balls[3].moveTo(__players[movesTeam1[i][0]].getPosition()); //superposition joueur et balle
                }
            }
            else if((__players[movesTeam1[i][0]].getRole() == ROLE_SEEKER) and (movesTeam1[i][1] == CATCH_GOLDENSNITCH)){ //normalement ce test est fait aussi niveau client
                //tentative d'attraper le vif d'or
                //calcul de la distance entre l'attrapeur et la balle
                int distance = __players[movesTeam1[i][0]].getPosition().getDistanceTo(__balls[0].getPosition());
                if(__players[movesTeam1[i][0]].catchGoldenSnitch(distance)){ //si attrapage réussi
                    __scoreTeam1 += 150;
                    __winner = (__scoreTeam1 > __scoreTeam2) ? 1 : 2;
                }
            }
        }
        if(movesTeam2[i][1] >= 0 and not __winner){ //tentative de capture d'une balle
            if((__players[movesTeam2[i][0]].getRole() == ROLE_CHASER) and (movesTeam2[i][1] == INTERCEPT_QUAFFLE)){ //normalement ce test est fait aussi niveau client
                //tentative de récupérer le quaffle
                //calcul de la distance entre l'attrapeur et la balle
                int distance = __players[movesTeam2[i][0]].getPosition().getDistanceTo(__players[QUAFFLE].getPosition());
                if(__players[movesTeam2[i][0]].interceptQuaffle(distance)){ //si attrapage réussi
                    __players[movesTeam2[i][0]].carryQuaffle(); //le joueur prend la balle avec lui
                    __field.setOccupant(__players[QUAFFLE].getPosition(), -1); //balle sort du terrain
                    __players[QUAFFLE].moveTo(__players[movesTeam2[i][0]].getPosition()); //superposition joueur et balle
                }
            }
            else if((__players[movesTeam2[i][0]].getRole() == ROLE_SEEKER) and (movesTeam2[i][1] == CATCH_GOLDENSNITCH)){ //normalement ce test est fait aussi niveau client
                //tentative d'attraper le vif d'or
                //calcul de la distance entre l'attrapeur et la balle
                int distance = __players[movesTeam2[i][0]].getPosition().getDistanceTo(__players[GOLDENSNITCH].getPosition());
                if(__players[movesTeam2[i][0]].catchGoldenSnitch(distance) and not __winner){ //si attrapage réussi
                    __scoreTeam1 += 150;
                    __winner = (__scoreTeam1 > __scoreTeam2) ? 1 : 2;
                }
            }
        }
    }
    
    if(__winner) return;
    
    //2ème étape : gestion des déplacements case par case dans un ordre prioritaire
    
    std::vector< std::vector<Move> > allMoves;
    //TODO : analyser les objets dans movesTeam1 et movesTeam2 pour récupérer les destination par rapport à l'ordre prioritaire.
    //TODO : concaténer l'ensemble des déplacements voulu et les trier par rapport à l'ordre prioritaire par rapport à la vitesse
    //provisoire : réalisation des mouvements en fonction de l'ordre reçu des clients
    std::vector<int> movesOrder;
    
    for(int i = 0; i < 7; ++i){
        //on prend un joueur/balle déplacé par la 1ère équipe
        if((movesTeam1[i][1] == NO_SPECIAL_ACTION) and (movesTeam1[i][2] < 10000)){ //si pas d'action spéciale et déplacement non vide
            std::cout << "Objet de la 1ère équipe" << std::endl;
            if(movesTeam1[i][0] < GOLDENSNITCH){ //si pas une balle
                std::vector<Move> orderedMoves = __players[movesTeam1[i][0]].getPosition().getMovesTo(AxialCoordinates(movesTeam1[i][2], movesTeam1[i][3]));
                allMoves.push_back(orderedMoves);
                
            }else{
                std::vector<Move> orderedMoves = __balls[movesTeam1[i][0]].getPosition().getMovesTo(AxialCoordinates(movesTeam1[i][2], movesTeam1[i][3]));
                allMoves.push_back(orderedMoves);
            }
            movesOrder.push_back(movesTeam1[i][0]);
        }
        //on prend un joueur/balle à déplacer dans la seconde équipe
        if((movesTeam2[i][1] == NO_SPECIAL_ACTION) and (movesTeam2[i][2] < 10000)){ //si pas d'action spéciale et déplacement non vide
            if(movesTeam2[i][0] < GOLDENSNITCH){ //si pas une balle
                std::vector<Move> orderedMoves = __players[movesTeam2[i][0]].getPosition().getMovesTo(AxialCoordinates(movesTeam2[i][2], movesTeam2[i][3]));
                allMoves.push_back(orderedMoves);
                
            }else{
                std::vector<Move> orderedMoves = __balls[movesTeam2[i][0]].getPosition().getMovesTo(AxialCoordinates(movesTeam2[i][2], movesTeam2[i][3]));
                allMoves.push_back(orderedMoves);
            }
            movesOrder.push_back(movesTeam2[i][0]);
        }
    }
    // déplacement autonome
    movesOrder.push_back(GOLDENSNITCH);
    std::vector<Move> orderedMoves = __balls[0].getPosition().getMovesTo(__balls[0].getPosition().getRandomDestination(GOLDENSNITCH_SPEED));
    allMoves.push_back(orderedMoves);
    //TODO : déplacements autonomes des Bludger si pas frappé par un joueur
    std::cout << "On passe aux déplacements" << std::endl;
    std::cout << "Contenu du vecteur movesOrder " << std::endl;
    for(unsigned int i = 0; i < movesOrder.size(); ++i){
        std::cout << movesOrder[i] << std::endl;
    }
    std::cout << "taille du vecteur allMoves " << allMoves.size() <<  std::endl;
    std::cout << "vitesse vif d'or " << GOLDENSNITCH_SPEED <<  std::endl;
    std::cout << "taille du vecteur allMoves[2] " << allMoves[2].size() <<  std::endl;
    std::cout << "taille du vecteur allMoves[1] " << allMoves[1].size() <<  std::endl;
    std::cout << "taille du vecteur allMoves[0] " << allMoves[0].size() <<  std::endl;
    //réalisation des déplacements
    bool stillMoves = true;
    unsigned int moveIndex = 0;
    while(stillMoves){
        std::cout << "taille du vecteur allMoves[2] " << allMoves[2].size() <<  std::endl;
        stillMoves = false;
        for(unsigned int objectToMove = 0; objectToMove < movesOrder.size(); ++objectToMove){
            std::cout << "Dans le for "  << std::endl;
            if(allMoves[objectToMove].size() > moveIndex){ //si encore des déplacements unitaires pour cet objet
                stillMoves = true;
                std::cout << "Déplacement de " << movesOrder[objectToMove] << std::endl;
                
                if(movesOrder[objectToMove] < GOLDENSNITCH){ //si PlayingPlayer
                    AxialCoordinates currentPosition = __players[movesOrder[objectToMove]].getPosition();
                    std::cout << "départ : diag col " << currentPosition.getDiagAxis() << " " << currentPosition.getLineAxis() << std::endl;
                    AxialCoordinates destination(currentPosition.getDiagAxis() + allMoves[objectToMove][moveIndex].getDiagDiff(), currentPosition.getLineAxis() + allMoves[objectToMove][moveIndex].getLineDiff());
                    if(__field.getOccupant(destination) == FREE_SPACE){ //destination libre
                        __players[movesOrder[objectToMove]].moveTo(destination);
                        //TODO : méthodes + claires
                        __field.setOccupant(destination, movesOrder[objectToMove]);
                        __field.setOccupant(currentPosition, FREE_SPACE);
                    }
                    else if(__field.getOccupant(destination) < GOLDENSNITCH){ //collision avec un joueur : le mouvement s'arrête
                        allMoves[objectToMove].clear();
                    }else{//collision avec une balle
                        if((__field.getOccupant(destination) == BLUDGER1) or (__field.getOccupant(destination) == BLUDGER2)){
                            __players[movesOrder[objectToMove]].handleBludger();
                            if(__players[movesOrder[objectToMove]].hasQuaffle()){ 
                                __players[movesOrder[objectToMove]].loseQuaffle(); //lache le souaffle
                                //TODO : redonner une position et resituer le souaffle sur le terrain
                                //TODO : trouver d'autres manières de faire perdre le souaffle
                            }
                            if(__players[movesOrder[objectToMove]].getLife() == 0){ //abbatu par le cognard : sort du terrain
                                __players[movesOrder[objectToMove]].moveTo(AxialCoordinates(10000,10000)); //hors du jeu
                                __field.setOccupant(currentPosition, FREE_SPACE);
                            }
                            
                        }
                        allMoves[objectToMove].clear(); //de toute façon interrompu
                    }
                }else{ //si balle
                    AxialCoordinates currentPosition = __balls[movesOrder[objectToMove] - GOLDENSNITCH].getPosition();
                    AxialCoordinates destination(currentPosition.getDiagAxis() + allMoves[objectToMove][moveIndex].getDiagDiff(), currentPosition.getLineAxis() + allMoves[objectToMove][moveIndex].getLineDiff());
                    if(__field.getOccupant(destination) == FREE_SPACE){ //destination libre
                        __balls[movesOrder[objectToMove] - GOLDENSNITCH].moveTo(destination);
                        //TODO : méthodes + claires
                        __field.setOccupant(destination, movesOrder[objectToMove]);
                        __field.setOccupant(currentPosition, FREE_SPACE);
                        if(movesOrder[objectToMove] == QUAFFLE){ //test si traverse le but
                            if((destination.getDiagAxis() == GOAL_SIDE1_DIAG) and (destination.getLineAxis() == GOAL_SIDE1_LINE)){
                                __scoreTeam2 += 10;
                            }else if((destination.getDiagAxis() == GOAL_SIDE2_DIAG) and (destination.getLineAxis() == GOAL_SIDE2_LINE)){
                                ++__scoreTeam1 += 10;
                            }
                        }
                    }
                    else if(__field.getOccupant(destination) < GOLDENSNITCH){ //collision avec un joueur : le mouvement s'arrête
                        if((movesOrder[objectToMove] == BLUDGER1) or (movesOrder[objectToMove] == BLUDGER2)){
                            __players[__field.getOccupant(destination)].handleBludger();
                            if(__players[__field.getOccupant(destination)].hasQuaffle()){
                                __players[__field.getOccupant(destination)].loseQuaffle();
                                //TODO : redonner une position et resituer le souaffle sur le terrain
                            }
                            if(__players[__field.getOccupant(destination)].getLife() == 0){ //abbatu par le cognard : sort du terrain
                                __players[__field.getOccupant(destination)].moveTo(AxialCoordinates(10000,10000)); //hors du jeu
                                __field.setOccupant(destination, FREE_SPACE);
                            }
                        }
                        allMoves[objectToMove].clear();
                    }
                    else{//collision d'une balle avec une balle : déplacement s'arrête
                        allMoves[objectToMove].clear();
                    }
                }
            }
        }
        ++moveIndex;
        std::cout << "taille du vecteur allMoves[2] " << allMoves[2].size() <<  std::endl;
        std::cout << "stillMoves " << stillMoves <<  std::endl;
        std::cout << "moveIndex " << moveIndex <<  std::endl;
    }
    //for tests :
    __field.display();
}

bool Match::isInVector(std::vector<int> toTest, int value){
    bool result = false;
    for(unsigned int i = 0; i < toTest.size(); ++i){
        if(toTest[i] == value) result = true;
    }
    return result;
}

void Match::serializeScoreAndPositions(char * bufferPosition){
    memcpy(bufferPosition, &__winner, sizeof(__winner));
    bufferPosition += sizeof(__winner);
    memcpy(bufferPosition, &__scoreTeam1, sizeof(__scoreTeam1));
    bufferPosition += sizeof(__scoreTeam1);
    memcpy(bufferPosition, &__scoreTeam2, sizeof(__scoreTeam2));
    bufferPosition += sizeof(__scoreTeam2);
    for(unsigned int i = 0; i < __players.size(); ++i){ //positions des joueurs
        AxialCoordinates position = __players[i].getPosition();
        int diag = position.getDiagAxis();
        int line = position.getLineAxis();
        memcpy(bufferPosition, &diag, sizeof(diag));
        bufferPosition += sizeof(diag);
        memcpy(bufferPosition, &line, sizeof(line));
        bufferPosition += sizeof(line);
    }
    for(unsigned int i = 0; i < __balls.size(); ++i){ //positions des balles
        AxialCoordinates position = __balls[i].getPosition();
        int diag = position.getDiagAxis();
        int line = position.getLineAxis();
        memcpy(bufferPosition, &diag, sizeof(diag));
        bufferPosition += sizeof(diag);
        memcpy(bufferPosition, &line, sizeof(line));
        bufferPosition += sizeof(line);
    }
}

void Match::serializePlayerAttr(int playerID, char * bufferPosition){
    //on suppose l'ID valide
    __players[playerID].serializeAttributes(bufferPosition);
}
