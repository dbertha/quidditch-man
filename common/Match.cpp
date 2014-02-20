#include "Match.hpp"


//makeMoves :
//si quaffle passe par un but, augmenter score
//si seeker(attrapeur)
//si keeper (gardien)
//si chaser (poursuiveur : quaffle)
//si beater (batteur)
//si bludger collisionne joueur, handlerBludger

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
        __players.push_back(PlayingPlayer(team1[i], role, position));
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
        __players.push_back(PlayingPlayer(team2[i], role, position));
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
}

void Match::makeMoves(int movesTeam1[][4], int movesTeam2[][4]){
    //moves[][0] : indice de l'objet concerné
    //moves[][1] : action spéciale
    //moves[][2] : diagonale destination
    //moves[][3] : ligne destination
    //1ère étape : gestion des interceptions de balle
    for(int i = 0; i < 7; ++i){
        if(movesTeam1[i][1] > 0){ //tentative de capture d'une balle
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
            else if((__players[movesTeam1[i][0]].getRole() == ROLE_SEEKER) and (movesTeam1[i][1] == CATCH_GOLDENSNITCH)){ //normalement c'est test est fait aussi niveau client
                //tentative d'attraper le vif d'or
                //calcul de la distance entre l'attrapeur et la balle
                int distance = __players[movesTeam1[i][0]].getPosition().getDistanceTo(__balls[0].getPosition());
                if(__players[movesTeam1[i][0]].catchGoldenSnitch(distance)){ //si attrapage réussi
                    __winner = 1;
                }
            }
        }
        if(movesTeam2[i][1] > 0){ //tentative de capture d'une balle
            if((__players[movesTeam2[i][0]].getRole() == ROLE_CHASER) and (movesTeam2[i][1] == INTERCEPT_QUAFFLE)){ //normalement c'est test est fait aussi niveau client
                //tentative de récupérer le quaffle
                //calcul de la distance entre l'attrapeur et la balle
                int distance = __players[movesTeam2[i][0]].getPosition().getDistanceTo(__players[QUAFFLE].getPosition());
                if(__players[movesTeam2[i][0]].interceptQuaffle(distance)){ //si attrapage réussi
                    __players[movesTeam2[i][0]].carryQuaffle(); //le joueur prend la balle avec lui
                    __field.setOccupant(__players[QUAFFLE].getPosition(), -1); //balle sort du terrain
                    __players[QUAFFLE].moveTo(__players[movesTeam2[i][0]].getPosition()); //superposition joueur et balle
                }
            }
            else if((__players[movesTeam2[i][0]].getRole() == ROLE_SEEKER) and (movesTeam2[i][1] == CATCH_GOLDENSNITCH)){ //normalement c'est test est fait aussi niveau client
                //tentative d'attraper le vif d'or
                //calcul de la distance entre l'attrapeur et la balle
                int distance = __players[movesTeam2[i][0]].getPosition().getDistanceTo(__players[GOLDENSNITCH].getPosition());
                if(__players[movesTeam2[i][0]].catchGoldenSnitch(distance) and not __winner){ //si attrapage réussi
                    __winner = 2;
                }
            }
        }
    }
    
    //2ème étape : gestion des déplacements dans un ordre prioritaire
    
    
}
