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
#define UPDRADE_CONFIRM 6
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
