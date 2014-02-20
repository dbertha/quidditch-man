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
    
    for(int i = 0; i < 7; ++i){
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
    __field.display();
}
