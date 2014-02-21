#ifndef DEFINES_HPP
#define DEFINES_HPP

#define PORT 3495
#define BACKLOG 20 //nbre max de clients dans la file d'attente
#define ERROR -1
#define INPUTSIZE 100 //taille du message envoyé du client vers le serveur
#define __DEBUG

#define USERNAME_LENGTH 30
#define PASSWORD_LENGTH 30

#define LOGIN 1
#define LOGIN_CONFIRM 2
#define GETBUILDINGINFOS 3
#define BUILDINGINFOS 4
#define UPGRADE_BUILDING 5
#define UPGRADE_CONFIRM 6
#define PROPOSEMATCH 7
#define MATCH_CONFIRM 8
#define ACCEPTMATCH 9
//#define MATCH_CONFIRM 10
#define CREATEAUCTION 11
#define AUCTIONCREATION_CONFIRM 12
#define JOINAUCTION 13
#define AUCTIONJOIN_CONFIRM 14
#define GETMANAGERSLIST 15
#define MANAGERSLIST 16
#define GETAUCTIONSLIST 17
#define AUCTIONSLIST 18
#define GETPOSITIONS 19
#define POSITIONS 20
#define SELECTPLAYER 21 
#define PLAYERINFOS 22
#define MAKEMOVES 23
#define MOVES_CONFIRM 24
#define GETCURRENTPRICE 25
#define CURRENTPRICE 26
#define BID 27
#define BID_CONFIRM 28
#define TRAIN_PLAYER 29
#define TRAINING_STARTED 30
#define IS_MATCH_WAITING 31
#define MATCH_WAITING 32
#define CREATE_MANAGER 33
#define GETPLAYERSLIST 34
#define PLAYERSLIST 35
#define GETPLAYERINFOS 36
#define GETMANAGERINFOS 37
#define MANAGERINFOS 38
#define HEAL_PLAYER 39
#define HEAL_STARTED 40

#define STADIUM 1
#define TRAININGCENTER 2
#define HOSPITAL 3
#define FANSHOP 4
#define CONSTRUCTIONSTARTED 1
#define ALREADYINCONSTRUCTION 2
#define NOTENOUGHMONEY 3

#define SPEED 0
#define STRENGTH 1
#define PRECISION 2
#define REFLEX 3
#define RESISTANCE 4

#define NOT_ON_HEX_GRID -2
#define FREE_SPACE -1
#define TEAM1_KEEPER 0
#define TEAM1_SEEKER 1
#define TEAM1_CHASER1 2
#define TEAM1_CHASER2 3
#define TEAM1_CHASER3 4
#define TEAM1_BEATER1 5
#define TEAM1_BEATER2 6
#define TEAM2_KEEPER 7
#define TEAM2_SEEKER 8
#define TEAM2_CHASER1 9
#define TEAM2_CHASER2 10
#define TEAM2_CHASER3 11
#define TEAM2_BEATER1 12
#define TEAM2_BEATER2 13
#define GOLDENSNITCH 14
#define BLUDGER1 15
#define BLUDGER2 16
#define QUAFFLE 17

#define TEAM1_KEEPER_UNICODE "\u2780"
#define TEAM1_SEEKER_UNICODE "\u2781"
#define TEAM1_CHASER1_UNICODE "\u2782"
#define TEAM1_CHASER2_UNICODE "\u2783"
#define TEAM1_CHASER3_UNICODE "\u2784"
#define TEAM1_BEATER1_UNICODE "\u2785"
#define TEAM1_BEATER2_UNICODE "\u2786"
#define TEAM2_KEEPER_UNICODE "\u278A"
#define TEAM2_SEEKER_UNICODE "\u278B"
#define TEAM2_CHASER1_UNICODE "\u278C"
#define TEAM2_CHASER2_UNICODE "\u278D"
#define TEAM2_CHASER3_UNICODE "\u278E"
#define TEAM2_BEATER1_UNICODE "\u278F"
#define TEAM2_BEATER2_UNICODE "\u2790"
#define GOLDENSNITCH_UNICODE "\u2A00"
#define BLUDGER1_UNICODE "\u2A01"
#define BLUDGER2_UNICODE "\u2A01"
#define QUAFFLE_UNICODE "\u2B24"

#define GOAL_SIDE1_DIAG 9
#define GOAL_SIDE1_LINE 0
#define GOAL_SIDE2_DIAG -9
#define GOAL_SIDE2_LINE 0

#define ROLE_KEEPER 0
#define ROLE_SEEKER 1
#define ROLE_CHASER 2
#define ROLE_BEATER 3

#define INTERCEPT_QUAFFLE 0
#define CATCH_GOLDENSNITCH 1

//TODO : définir les positions de départ en fonction de la taille du terrain
//TODO : plusieurs buts quand grand terrain
//TODO : position de départ du vif d'or et des bugler aléatoire



#define STARTINGDIAG_QUAFFLE 0
#define STARTINGLINE_QUAFFLE 0

#define STARTINGDIAG_BLUDGER1 5
#define STARTINGLINE_BLUDGER1 5

#define STARTINGDIAG_BLUDGER2 -5
#define STARTINGLINE_BLUDGER2 -5

#define STARTINGDIAG_GOLDENSNITCH 2
#define STARTINGLINE_GOLDENSNITCH -3



#define STARTINGDIAG_TEAM1_KEEPER -8
#define STARTINGLINE_TEAM1_KEEPER 0

#define STARTINGDIAG_TEAM1_SEEKER -9
#define STARTINGLINE_TEAM1_SEEKER 9

#define STARTINGDIAG_TEAM1_CHASER1 0
#define STARTINGLINE_TEAM1_CHASER1 -6

#define STARTINGDIAG_TEAM1_CHASER2 -3
#define STARTINGLINE_TEAM1_CHASER2 0

#define STARTINGDIAG_TEAM1_CHASER3 -6
#define STARTINGLINE_TEAM1_CHASER3 6

#define STARTINGDIAG_TEAM1_BEATER1 1
#define STARTINGLINE_TEAM1_BEATER1  -5

#define STARTINGDIAG_TEAM1_BEATER2 -4
#define STARTINGLINE_TEAM1_BEATER2 5

#define STARTINGDIAG_TEAM2_KEEPER 8
#define STARTINGLINE_TEAM2_KEEPER 0

#define STARTINGDIAG_TEAM2_SEEKER 9
#define STARTINGLINE_TEAM2_SEEKER -9
 
#define STARTINGDIAG_TEAM2_CHASER1 6
#define STARTINGLINE_TEAM2_CHASER1 -6

#define STARTINGDIAG_TEAM2_CHASER2 3
#define STARTINGLINE_TEAM2_CHASER2 0

#define STARTINGDIAG_TEAM2_CHASER3 0
#define STARTINGLINE_TEAM2_CHASER3 6

#define STARTINGDIAG_TEAM2_BEATER1 4
#define STARTINGLINE_TEAM2_BEATER1 -5

#define STARTINGDIAG_TEAM2_BEATER2 -1
#define STARTINGLINE_TEAM2_BEATER2 5

#endif
