#include "Match.hpp"


Match::Match(std::vector<ManagedPlayer> &team1, int ** movesTeam1, bool tournament): 
    __players(), __field(), __winner(0), __movesTeam1(movesTeam1), __movesTeam2(NULL), __isInTournament(tournament){
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

void Match::launch(std::vector<ManagedPlayer> &team2, int ** movesTeam2){ //suite du constructeur
#ifdef __DEBUG
        std::cout << "Match démarré avec la réception de la 2 ème équipe" << std::endl;
#endif
    __movesTeam2 = movesTeam2;
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
    __balls.push_back(Ball(BLUDGER2, AxialCoordinates(STARTINGDIAG_BLUDGER2, STARTINGLINE_BLUDGER2))); 
    __field.setOccupant(AxialCoordinates(STARTINGDIAG_BLUDGER2, STARTINGLINE_BLUDGER2), BLUDGER2);
    __balls.push_back(Ball(QUAFFLE, AxialCoordinates(STARTINGDIAG_QUAFFLE, STARTINGLINE_QUAFFLE))); 
    __field.setOccupant(AxialCoordinates(STARTINGDIAG_QUAFFLE, STARTINGLINE_QUAFFLE), QUAFFLE);
#ifdef __DEBUG
    __field.display();
    std::cout << "On passe aux priorités." << std::endl;
#endif
    
    sortBySpeed();
#ifdef __DEBUG
    std::cout << "Fin priorités." << std::endl;
#endif
}

void Match::launchTrainingMatch(){
#ifdef __DEBUG
        std::cout << "Match d'entrainement initialisé" << std::endl;
#endif
    //creation de l'équipe de l'IA :
    
    AxialCoordinates position;
    ManagedPlayer player;
    
    position = AxialCoordinates(STARTINGDIAG_TEAM2_KEEPER, STARTINGLINE_TEAM2_KEEPER);
    DataBase::loadPlayerFrom("server/Saves/IAKeeper.txt", &player);
    __players.push_back(PlayingPlayer(player, ROLE_KEEPER, position));
    __field.setOccupant(position, TEAM2_KEEPER);
    
    position = AxialCoordinates(STARTINGDIAG_TEAM2_SEEKER, STARTINGLINE_TEAM2_SEEKER);
    DataBase::loadPlayerFrom("server/Saves/IASeeker.txt", &player);
    __players.push_back(PlayingPlayer(player, ROLE_SEEKER, position));
    __field.setOccupant(position, TEAM2_SEEKER);
    
    position = AxialCoordinates(STARTINGDIAG_TEAM2_CHASER1, STARTINGLINE_TEAM2_CHASER1);
    DataBase::loadPlayerFrom("server/Saves/IAChaser.txt", &player);
    __players.push_back(PlayingPlayer(player, ROLE_CHASER, position));
    __field.setOccupant(position, TEAM2_CHASER1);
    position = AxialCoordinates(STARTINGDIAG_TEAM2_CHASER2, STARTINGLINE_TEAM2_CHASER2);
    DataBase::loadPlayerFrom("server/Saves/IAChaser.txt", &player);
    __players.push_back(PlayingPlayer(player, ROLE_CHASER, position));
    __field.setOccupant(position, TEAM2_CHASER2);
    position = AxialCoordinates(STARTINGDIAG_TEAM2_CHASER3, STARTINGLINE_TEAM2_CHASER3);
    DataBase::loadPlayerFrom("server/Saves/IAChaser.txt", &player);
    __players.push_back(PlayingPlayer(player, ROLE_CHASER, position));
    __field.setOccupant(position, TEAM2_CHASER3);
    
    position = AxialCoordinates(STARTINGDIAG_TEAM2_BEATER1, STARTINGLINE_TEAM2_BEATER1);
    DataBase::loadPlayerFrom("server/Saves/IABeater.txt", &player);
    __players.push_back(PlayingPlayer(player, ROLE_BEATER, position));
    __field.setOccupant(position, TEAM2_BEATER1);
    position = AxialCoordinates(STARTINGDIAG_TEAM2_BEATER2, STARTINGLINE_TEAM2_BEATER2);
    DataBase::loadPlayerFrom("server/Saves/IABeater.txt", &player);
    __players.push_back(PlayingPlayer(player, ROLE_BEATER, position));
    __field.setOccupant(position, TEAM2_BEATER2);
    
    __movesTeam2 = new int*[7];
    for(int i = 0; i < 7; ++i){
        __movesTeam2[i] = new int[4];
        __movesTeam2[i][0] = i + TEAM2_KEEPER;
        __movesTeam2[i][1] = NO_SPECIAL_ACTION;
        __movesTeam2[i][2] = 10000;
        __movesTeam2[i][3] = 10000;
    }
    //action systématique : essayer d'attraper le vif d'or
    __movesTeam2[1][0] = TEAM2_SEEKER;
    __movesTeam2[1][1] = CATCH_GOLDENSNITCH;
    
    //les 4 balles :
    __balls.push_back(Ball(GOLDENSNITCH, AxialCoordinates(STARTINGDIAG_GOLDENSNITCH, STARTINGLINE_GOLDENSNITCH))); 
    __field.setOccupant(AxialCoordinates(STARTINGDIAG_GOLDENSNITCH, STARTINGLINE_GOLDENSNITCH), GOLDENSNITCH);
    __balls.push_back(Ball(BLUDGER1, AxialCoordinates(STARTINGDIAG_BLUDGER1, STARTINGLINE_BLUDGER1))); 
    __field.setOccupant(AxialCoordinates(STARTINGDIAG_BLUDGER1, STARTINGLINE_BLUDGER1), BLUDGER1);
    __balls.push_back(Ball(BLUDGER2, AxialCoordinates(STARTINGDIAG_BLUDGER2, STARTINGLINE_BLUDGER2))); 
    __field.setOccupant(AxialCoordinates(STARTINGDIAG_BLUDGER2, STARTINGLINE_BLUDGER2), BLUDGER2);
    __balls.push_back(Ball(QUAFFLE, AxialCoordinates(STARTINGDIAG_QUAFFLE, STARTINGLINE_QUAFFLE))); 
    __field.setOccupant(AxialCoordinates(STARTINGDIAG_QUAFFLE, STARTINGLINE_QUAFFLE), QUAFFLE);
#ifdef __DEBUG
    __field.display();
    std::cout << "On passe aux priorités." << std::endl;
#endif
    
    sortBySpeed();
#ifdef __DEBUG
    std::cout << "Fin priorités." << std::endl;
#endif
}

void Match::deleteIATeam(){
    for(int i =0; i < 7; ++i){
		delete[] __movesTeam2[i];
    }
    delete[] __movesTeam2;
}

void Match::sortBySpeed(){
    //on enregistre les priorités de déplacements :
    //TODO : à optimiser
    int maxIndex, maxSpeedValue;
    for(int i = 0; i < 18; ++i){
        maxIndex = -1;
        maxSpeedValue = -1;
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

Match::Match(std::vector<ManagedPlayer> &team1, std::vector<ManagedPlayer> &team2, int ** movesTeam1, int ** movesTeam2) :
    __players(), __field(), __winner(0), __movesTeam1(movesTeam1), __movesTeam2(movesTeam2){
    
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
    __balls.push_back(Ball(BLUDGER2, AxialCoordinates(STARTINGDIAG_BLUDGER2, STARTINGLINE_BLUDGER2))); 
    __field.setOccupant(AxialCoordinates(STARTINGDIAG_BLUDGER2, STARTINGLINE_BLUDGER2), BLUDGER2);
    __balls.push_back(Ball(QUAFFLE, AxialCoordinates(STARTINGDIAG_QUAFFLE, STARTINGLINE_QUAFFLE))); 
    __field.setOccupant(AxialCoordinates(STARTINGDIAG_QUAFFLE, STARTINGLINE_QUAFFLE), QUAFFLE);
    __field.display();
    
    sortBySpeed();
    
}

void Match::makeMoves(){
    //moves[][0] : indice de l'objet concerné
    //moves[][1] : action spéciale
    //moves[][2] : diagonale destination
    //moves[][3] : ligne destination
    //1ère étape : gestion des interceptions de balle
    
    for(int i = 0; i < 7; ++i){
        if(__movesTeam1[i][1] >= 0){ //tentative de capture d'une balle
            if((__players[__movesTeam1[i][0]].getRole() == ROLE_CHASER) and (__movesTeam1[i][1] == INTERCEPT_QUAFFLE)){ //normalement c'est test est fait aussi niveau client
                //tentative de récupérer le souaffle
                //calcul de la distance entre l'attrapeur et la balle
#ifdef __DEBUG
                std::cout << "Tentative d'attraper le souaffle" << std::endl;
#endif
                int distance = __players[__movesTeam1[i][0]].getDistanceTo(__balls[3].getPosition());
                if(__players[__movesTeam1[i][0]].interceptQuaffle(distance)){ //si attrapage réussi
#ifdef __DEBUG
                    std::cout << "Capture du souaffle réussie" << std::endl;
#endif
                    __players[__movesTeam1[i][0]].carryQuaffle(); //le joueur prend la balle avec lui
                    __field.setOccupant(__balls[3].getPosition(), FREE_SPACE); //balle sort du terrain
                    __balls[3].moveTo(__players[__movesTeam1[i][0]].getPosition()); //superposition joueur et balle
                }
            }
            else if((__players[__movesTeam1[i][0]].getRole() == ROLE_SEEKER) and (__movesTeam1[i][1] == CATCH_GOLDENSNITCH)){ //normalement ce test est fait aussi niveau client
                //tentative d'attraper le vif d'or
                //calcul de la distance entre l'attrapeur et la balle
#ifdef __DEBUG
                    std::cout << "Tentative de capture du vif d'or" << std::endl;
#endif
                int distance = __players[__movesTeam1[i][0]].getDistanceTo(__balls[0].getPosition());
                if(__players[__movesTeam1[i][0]].catchGoldenSnitch(distance)){ //si attrapage réussi
#ifdef __DEBUG
                    std::cout << "Tentative de capture du vif d'or réussie" << std::endl;
#endif
                    __scoreTeam1 += 150;
                    __winner = (__scoreTeam1 > __scoreTeam2) ? 1 : 2;
                }
            }
        }
        if(__movesTeam2[i][1] >= 0 and not __winner){ //tentative de capture d'une balle
            if((__players[__movesTeam2[i][0]].getRole() == ROLE_CHASER) and (__movesTeam2[i][1] == INTERCEPT_QUAFFLE)){ //normalement ce test est fait aussi niveau client
                //tentative de récupérer le souaffle
#ifdef __DEBUG
                std::cout << "Tentative d'attraper le souaffle" << std::endl;
#endif
                //calcul de la distance entre l'attrapeur et la balle
                int distance = __players[__movesTeam2[i][0]].getDistanceTo(__balls[3].getPosition());
                if(__players[__movesTeam2[i][0]].interceptQuaffle(distance)){ //si attrapage réussi
#ifdef __DEBUG
                    std::cout << "Capture du souaffle réussie" << std::endl;
#endif
                    __players[__movesTeam2[i][0]].carryQuaffle(); //le joueur prend la balle avec lui
                    int occupant = __field.getOccupant(__balls[3].getPosition());
                    if((occupant > FREE_SPACE) and (occupant < GOLDENSNITCH)){ //si la balle était déjà possédée par un joueur
                        __players[occupant].loseQuaffle();
                    }else{
                        __field.setOccupant(__balls[3].getPosition(), FREE_SPACE); //balle sort du terrain
                    }
                    __balls[3].moveTo(__players[__movesTeam2[i][0]].getPosition()); //superposition joueur et balle
                }
            }
            else if((__players[__movesTeam2[i][0]].getRole() == ROLE_SEEKER) and (__movesTeam2[i][1] == CATCH_GOLDENSNITCH)){ //normalement ce test est fait aussi niveau client
                //tentative d'attraper le vif d'or
#ifdef __DEBUG
                    std::cout << "Tentative de capture du vif d'or" << std::endl;
#endif
                //calcul de la distance entre l'attrapeur et la balle
                int distance = __players[__movesTeam2[i][0]].getDistanceTo(__players[GOLDENSNITCH].getPosition());
                if(__players[__movesTeam2[i][0]].catchGoldenSnitch(distance) and not __winner){ //si attrapage réussi
#ifdef __DEBUG
                    std::cout << "Tentative de capture du vif d'or réussie" << std::endl;
#endif
                    __scoreTeam2 += 150;
                    __winner = (__scoreTeam1 > __scoreTeam2) ? 1 : 2;
                }
            }
        }
    }
    
    if(__winner) return;
    
    //2ème étape : gestion des déplacements case par case dans un ordre prioritaire
    
    std::vector< std::vector<Move> > allMoves;
    std::vector< int > movesOrder;

    //TODO : test si on n'essaie pas de frapper 2 fois sur le même cognard : priorité au plus rapide
    //génération des déplacements
    for(unsigned int i = 0; i < __indexesSortedBySpeed.size(); ++i){
        int * currentMove = NULL;
        for(int move = 0; move < 7; ++move){
            if((__movesTeam1[move][0] == __indexesSortedBySpeed[i]) and (__movesTeam1[move][1] == NO_SPECIAL_ACTION) and (__movesTeam1[move][2] < 10000)) { //si pas d'action spéciale et déplacement non vide
                currentMove = __movesTeam1[move];
            }else if((__movesTeam2[move][0] == __indexesSortedBySpeed[i]) and (__movesTeam2[move][1] == NO_SPECIAL_ACTION) and (__movesTeam2[move][2] < 10000)){
                currentMove = __movesTeam2[move];
            }
        }
        if(currentMove != NULL) { //si le prochain dans la liste triée par vitesse a bien reçu un déplacement
            movesOrder.push_back(currentMove[0]);
            if(currentMove[0] < GOLDENSNITCH){ //si pas une balle
                std::vector<Move> orderedMoves = __players[currentMove[0]].getMovesTo(AxialCoordinates(currentMove[2], currentMove[3]));
                allMoves.push_back(orderedMoves);
            }else{
                std::vector<Move> orderedMoves = __balls[currentMove[0] - GOLDENSNITCH].getMovesTo(AxialCoordinates(currentMove[2], currentMove[3]));
                allMoves.push_back(orderedMoves);
            }
        }
        else if(__indexesSortedBySpeed[i] == GOLDENSNITCH){
            // déplacement autonome du vif d'or
            std::vector<Move> orderedMoves = __balls[0].getMovesTo(__balls[0].getPosition().getRandomDestination(GOLDENSNITCH_SPEED));

            allMoves.push_back(orderedMoves);
            movesOrder.push_back(__indexesSortedBySpeed[i]);
        }
        else if((__indexesSortedBySpeed[i] == BLUDGER1) or (__indexesSortedBySpeed[i] == BLUDGER2)){
            // si pas cogné, cognards se déplacent de façon autonome
            std::vector<Move> orderedMoves = __balls[__indexesSortedBySpeed[i] - GOLDENSNITCH].getMovesTo(__balls[__indexesSortedBySpeed[i] - GOLDENSNITCH].getPosition().getRandomDestination(BLUDGER_AUTOSPEED));

            allMoves.push_back(orderedMoves);
            movesOrder.push_back(__indexesSortedBySpeed[i]);
        }
        else if(__indexesSortedBySpeed[i] == QUAFFLE){
            std::vector<Move> orderedMoves;

            allMoves.push_back(orderedMoves);
            movesOrder.push_back(__indexesSortedBySpeed[i]);
        }
    }
            
    
    
    

    //réalisation des déplacements
    bool stillMoves = true;
    unsigned int moveIndex = 0;
    while(stillMoves){
        stillMoves = false;
        for(unsigned int objectToMove = 0; objectToMove < movesOrder.size(); ++objectToMove){

            if(allMoves[objectToMove].size() > moveIndex){ //si encore des déplacements unitaires pour cet objet

                stillMoves = true;
                if(movesOrder[objectToMove] < GOLDENSNITCH){ //si PlayingPlayer
                    
                    AxialCoordinates currentPosition = __players[movesOrder[objectToMove]].getPosition();
                    if(currentPosition.getDiagAxis() == 1000){ //un joueur peut être abbatu par un déplacement de balle alors qu'il a encore des déplacements
                        allMoves[objectToMove].clear();
                    }
                    else{
                        AxialCoordinates destination(currentPosition.getDiagAxis() + allMoves[objectToMove][moveIndex].getDiagDiff(), currentPosition.getLineAxis() + allMoves[objectToMove][moveIndex].getLineDiff());
                        if(__field.getOccupant(destination) == FREE_SPACE){ //destination libre
                            __players[movesOrder[objectToMove]].moveTo(destination);
                            if(__players[movesOrder[objectToMove]].hasQuaffle()){
                                __balls[3].moveTo(destination); 
                                //souaffle se déplace avec joueur qui le porte, ce joueur ne peut se déplacer avec la balle et la lancer sur un même tour
                            }
                            //TODO : méthodes + claires
                            __field.setOccupant(destination, movesOrder[objectToMove]);
                            __field.setOccupant(currentPosition, FREE_SPACE);
                        }
                        else if((__field.getOccupant(destination) < GOLDENSNITCH) and (__field.getOccupant(destination) > NOT_ON_HEX_GRID)){ //collision avec un joueur : le mouvement s'arrête
                            allMoves[objectToMove].clear();
                        }else{//collision avec une balle
                            if((__field.getOccupant(destination) == BLUDGER1) or (__field.getOccupant(destination) == BLUDGER2)){
                                __players[movesOrder[objectToMove]].handleBludger();
                                if(__players[movesOrder[objectToMove]].hasQuaffle()){ 
                                    __players[movesOrder[objectToMove]].loseQuaffle(); //lache le souaffle
                                    AxialCoordinates quafflePosition = __players[movesOrder[objectToMove]].getPosition();
                                    //souaffle libéré dans une case adjacente
                                    quafflePosition = AxialCoordinates(quafflePosition.getDiagAxis(), quafflePosition.getLineAxis() + 1); //case devant
                                    if((not quafflePosition.isOnField()) or (__field.getOccupant(quafflePosition) != FREE_SPACE)){
                                        quafflePosition = AxialCoordinates(quafflePosition.getDiagAxis(), quafflePosition.getLineAxis() - 1); //case derrière
                                    }
                                    __field.setOccupant(quafflePosition, QUAFFLE);
                                    __balls[3].moveTo(quafflePosition);
                                }
                                if(__players[movesOrder[objectToMove]].getLife() == 0){ //abbatu par le cognard : sort du terrain
    #ifdef __DEBUG
                        std::cout << "Joueur abbatu : " << movesOrder[objectToMove] << std::endl;
    #endif
                                    __players[movesOrder[objectToMove]].moveTo(AxialCoordinates(10000,10000)); //hors du jeu
                                    __field.setOccupant(currentPosition, FREE_SPACE);
                                }
                                
                            }
                            allMoves[objectToMove].clear(); //de toute façon interrompu
                        }
                    }
                }else{ //si balle
                    AxialCoordinates currentPosition = __balls[movesOrder[objectToMove] - GOLDENSNITCH].getPosition();
                    AxialCoordinates destination(currentPosition.getDiagAxis() + allMoves[objectToMove][moveIndex].getDiagDiff(), currentPosition.getLineAxis() + allMoves[objectToMove][moveIndex].getLineDiff());
                    if(__field.getOccupant(destination) == FREE_SPACE){ //destination libre
                        if(moveIndex == 0 and movesOrder[objectToMove] == QUAFFLE){ //si premier déplacement du quaffle, gérer la superposition avec le joueur
                            //__field.setOccupant(currentPosition, FREE_SPACE); //détachement de la balle et du joueur la portant
                            __players[__field.getOccupant(currentPosition)].loseQuaffle(); //chaser qui avait la balle ne l'a plus
                        }else{
                            __field.setOccupant(currentPosition, FREE_SPACE);
                        }
                        __balls[movesOrder[objectToMove] - GOLDENSNITCH].moveTo(destination);
                        //TODO : méthodes + claires
                        __field.setOccupant(destination, movesOrder[objectToMove]);
                        
                        if(movesOrder[objectToMove] == QUAFFLE){ //test si traverse le but
                            //TODO : test trajectoire rectiligne : niveau client
                            //TODO : si collision avec le gardien, une chance de passer à travers
                            if(__field.isTeam1Goal(destination)){
                                __scoreTeam2 += 10;
                            }else if(__field.isTeam2Goal(destination)){
                                __scoreTeam1 += 10;
                            }
                        }
                    }
                    else if((__field.getOccupant(destination) < GOLDENSNITCH) and (__field.getOccupant(destination) > NOT_ON_HEX_GRID)){ //collision avec un joueur : le mouvement s'arrête
                        if((movesOrder[objectToMove] == BLUDGER1) or (movesOrder[objectToMove] == BLUDGER2)){
                            int occupantIndex = __field.getOccupant(destination);
                            __players[occupantIndex].handleBludger();
                            if(__players[occupantIndex].hasQuaffle()){
                                __players[occupantIndex].loseQuaffle();
                                AxialCoordinates quafflePosition = __players[occupantIndex].getPosition();
                                //souaffle libéré dans une case adjacente
                                quafflePosition = AxialCoordinates(quafflePosition.getDiagAxis(), quafflePosition.getLineAxis() + 1); //case devant
                                if((not quafflePosition.isOnField()) or (__field.getOccupant(quafflePosition) != FREE_SPACE)){
                                    quafflePosition = AxialCoordinates(quafflePosition.getDiagAxis(), quafflePosition.getLineAxis() - 1); //case derrière
                                }
                                __field.setOccupant(quafflePosition, QUAFFLE);
                                __balls[3].moveTo(quafflePosition);
                            
                            }
                            if(__players[__field.getOccupant(destination)].getLife() == 0){ //abbatu par le cognard : sort du terrain
#ifdef __DEBUG
                    std::cout << "Joueur abbatu : " << __field.getOccupant(destination) << std::endl;
#endif
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

    }
    for(unsigned int objectToMove = 0; objectToMove < movesOrder.size(); ++objectToMove){
        //on détruit tous les objets internes
        allMoves[objectToMove].clear();
    }
}

bool Match::isInVector(std::vector<int> toTest, int value){
    bool result = false;
    for(unsigned int i = 0; i < toTest.size(); ++i){
        if(toTest[i] == value) result = true;
    }
    return result;
}

int Match::serializeScoreAndPositions(char * bufferPosition){
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
    return __winner;
}

void Match::serializePlayerAttr(int playerID, char * bufferPosition){
    //on suppose l'ID valide
    __players[playerID].serializeAttributes(bufferPosition);
}

std::vector<int> Match::getLifesOfTeam(int nbTeam){
    std::vector<int> result;
    int indexStart = TEAM1_KEEPER;
    if(nbTeam == 2){
        indexStart = TEAM2_KEEPER;
    }
    for(int i = indexStart; i < (indexStart + 7); ++i){
        result.push_back(__players[i].getLife());
    }
    return result;
}

void Match::generateIAActions(){
    AxialCoordinates currentPosition, destination;
    for(int i = 0; i < 7; ++i){
        if(i != 1){ //SEEKER reçoit tjs la même action
            currentPosition = __players[i+TEAM2_KEEPER].getPosition();
            destination = currentPosition.getRandomDestination(__players[i+TEAM2_KEEPER].getCapacity(SPEED));
            __movesTeam2[i][2] = destination.getDiagAxis();
            __movesTeam2[i][3] = destination.getLineAxis();
        }
    }
}
