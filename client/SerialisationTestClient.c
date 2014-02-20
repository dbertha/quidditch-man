#define _BSD_SOURCE
#include "../common/NetworkBase.h"
#include "../common/NetworkInterface.hpp"
#include "../common/Defines.hpp"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/unistd.h>

#define DIM 100

void displayPlayersList() {}

void displayConnexionMenu(){
  printf(" ------------------------ WELCOME TO QUIDDITCH MANAGER 2014 ! ------------------------");
  printf("\n [1] Log in\n");
  printf(" [2] Create a new game\n");
  printf("-----> ");
}

void displayMainMenu(){
  printf(" --------------------------------------------------------------------------------------");
  printf("\nWhat do you want to do ?");
  printf("\n [1] See managers connected\n");
  printf(" [2] See/create auctions\n");
  printf(" [3] Manage players\n");
  printf(" [4] Manage buildings\n");
  printf(" [0] Quit game\n");
  printf("-----> ");
}

void displayAuctionMenus(){
  printf(" --------------------------------------------------------------------------------------");
  printf("\n//YOU'RE NOW IN THE AUCTION ROOM// What do you want to do ?");
  printf("\n [1] See current auctions\n");
  printf(" [2] Sell a player\n");
  printf(" [0] Return to the main menu\n");
  printf("-----> ");
}

void displaySellPlayerMenu(){
  printf(" --------------------------------------------------------------------------------------");
  printf("\n//YOU'RE ABOUT TO SELL A PLAYER// What do you want to do ?");
  displayPlayersList();
  printf("\n Which player do you want to sell [enter 0 to abort] ? ");
}

void displayManagePlayersMenu(){
  printf(" --------------------------------------------------------------------------------------");
  printf("\n//MANAGE TEAM// What do you want to do ?");
  printf("\n [1] Inspect a player\n");
  printf(" [2] Train a player\n");
  printf(" [3] Send a player to the hospital\n");
  printf(" [0] Return to the main menu\n");
  printf("-----> ");
}

void displayManageBuildingsMenu(){
  printf(" --------------------------------------------------------------------------------------");
  printf("\n//MANAGE BUILDINGS// What do you want to do ?");
  printf("\n [1] Enter Stadium\n");
  printf(" [2] Enter Training Center\n");
  printf(" [3] Enter Hospital\n");
  printf(" [4] Enter FanShop\n");
  printf(" [0] Return to the main menu\n");
  printf("-----> ");
}



int main(int argc, char *argv[]){
  int choice;
  int result;
  int buildingID, userID;
  int confirm, price, moves[7][3];
  int att2;
  long att3;
  char att4[43];
  char username[USERNAME_LENGTH];
  char password[PASSWORD_LENGTH];
  char * position;
  SerializedObject toReceiveFromServer;
  int sockfd;
  struct sockaddr_in server_addr; 
  struct hostent *server;
  char serverName[DIM];
  
  if (argc != 2) {
    if(argc == 1){ /*pas d'arguments : serveur est sur la machine locale */
      gethostname(serverName, 60);
      printf("Le server est la machine locale, dont le nom est %s\n", serverName);
    }else{
      fprintf(stderr, "Donner le nom de la machine"
	    " distante en argument.");
	    return EXIT_FAILURE;
    }
    
  }else{
    int i = 0;
    while((argv[i] != '\0') && (i < DIM)){
      serverName[i] = argv[1][i];
      ++i;
    }
    serverName[i] = '\0';
	  printf("Le nom du server distant est %s\n", serverName);
  }
  

  if ((server=gethostbyname(serverName)) == NULL) { 
    perror("Client: gethostbyname");
    return EXIT_FAILURE;
  }
  
  
  if ((sockfd = socket(PF_INET, 
		       SOCK_STREAM, 0)) == -1) {
    perror("Client: socket");
    return EXIT_FAILURE;
  }
  
  server_addr.sin_family = AF_INET;  
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr = *((struct in_addr*)server->h_addr);
  memset(&(server_addr.sin_zero), '\0', 8); 
  
  if (connect(sockfd, (struct sockaddr *) &server_addr,
	      sizeof(struct sockaddr)) == -1) {
    perror("Client: connect");
    return 0;
  }
  
  //~ if(recv(sockfd, &toReceiveFromServer, sizeof(SerializedObject), 0) != sizeof(SerializedObject)){
    //~ printf("%s\n", "Erreur dans la reception des données");
    //~ return ERROR;
  //~ }
  
  //~ printf("%s\n", "Contenu du buffer reçu du le réseau :");
  //~ 
  //~ toReceiveFromServer.typeOfInfos = ntohs(toReceiveFromServer.typeOfInfos);
  //~ printf("%s : %d\n", "type of infos", toReceiveFromServer.typeOfInfos);
  //~ toReceiveFromServer.senderId = ntohs(toReceiveFromServer.senderId);
  //~ printf("%s : %d\n", "sender ID", toReceiveFromServer.senderId);
  //~ toReceiveFromServer.numInSequence = ntohs(toReceiveFromServer.numInSequence);
  //~ printf("%s : %d\n", "num in sequence", toReceiveFromServer.numInSequence);
  //~ toReceiveFromServer.nbOfFollowing = ntohs(toReceiveFromServer.nbOfFollowing);
  //~ printf("%s : %d\n", "num of following", toReceiveFromServer.nbOfFollowing);
//~ 
//~ 
  //~ position = toReceiveFromServer.stringData;
  //~ memcpy(&att1, position, sizeof(att1));
  //~ position += sizeof(att1);
  //~ memcpy(&att2, position, sizeof(att2));
  //~ position += sizeof(att2);
  //~ memcpy(&att3, position, sizeof(att3));
  //~ position += sizeof(att3);
  //~ memcpy(&att4, position, sizeof(att4));
  //~ printf("%s\n", "attribut séquentialisés : ");
  //~ printf("%s : %d\n", "Entier 1 ", att1);
  //~ printf("%s : %d\n", "Entier 2 ", att2);
  //~ printf("%s : %d\n", "Long ", att3);
  //~ printf("%s : %s\n", "char[43] :  ", att4);
  
  //~ printf("%s\n", "Test du passage des infos de logging :");
  //~ printf ("Enter your username : (no spaces allowed)");
  //~ scanf ("%29s",username);  
  //~ printf ("Enter your password : ");
  //~ scanf ("%29s",password);  
  //~ 
  //~ sendLoginToServer(sockfd, username, password);
  //~ 
  //~ printf("%s\n", "Test du passage des infos de bâtiments :");
  //~ printf ("indice du bâtiment demandé : ");
  //~ scanf ("%d",&buildingID);  
  //~ 
  //~ askForBuildingInfos(sockfd, buildingID);
  //~ 
  //~ printf("%s\n", "Test du passage d'une demande d'upgrade de bâtiments :");
  //~ printf ("indice du bâtiment demandé : ");
  //~ scanf ("%d",&buildingID);  
  //~ 
  //~ askForBuildingUpgrade(sockfd, buildingID);
  //~ 
  //~ printf("%s\n", "Test du passage d'une proposition de match :");
  //~ printf ("indice de l'adversaire : ");
  //~ scanf ("%d",&userID);  
  //~ 
  //~ proposeMatchTo(sockfd, userID);
  //~ 
  //~ printf("%s\n", "Test d'une réponse à une proposition de match :");
  //~ printf ("indice du demandeur : ");
  //~ scanf ("%d",&userID);  
  //~ printf ("confirmation 0/1 : ");
  //~ scanf ("%d",&confirm);  
  //~ 
  //~ answerMatchProposal(sockfd, userID, confirm);
  //~ 
  //~ printf("%s\n", "Test si match en attente :");
  //~ 
  //~ isMatchWaiting(sockfd);
  //~ 
  //~ printf("%s\n", "Test du démarrage d'une vente aux enchères :");
  //~ printf ("indice du joueur à vendre : ");
  //~ scanf ("%d",&userID);  
  //~ printf ("prix de départ : ");
  //~ scanf ("%d",&price);  
  //~ 
  //~ sellPlayer(sockfd, userID, price);
  //~ 
  //~ printf("%s\n", "Test de la participation à une vente aux enchères :");
  //~ printf ("indice de l'enchère à rejoindre : ");
  //~ scanf ("%d",&userID);  
  //~ 
  //~ joinAuction(sockfd, userID);
  //~ 
  //~ printf("%s\n", "Test passage demande list joueurs :");
  //~ 
  //~ getManagersList(sockfd);
//~ 
  //~ printf("%s\n", "Test passage demande list enchères :");
  //~ 
  //~ getAuctionsList(sockfd);
  //~ 
  //~ printf("%s\n", "Test passage demande ensemble des positions sur le terrain :");
  //~ 
  //~ getAllPositions(sockfd);
  //~ 
  //~ printf("%s\n", "Test du passage de la sélection d'un joueur :");
  //~ printf ("indice du joueur : ");
  //~ scanf ("%d",&userID);  
  //~ selectPlayer(sockfd, userID);
  //~ 
  //~ printf("%s\n", "Test du passage d'une demande d'entrainement :");
  //~ printf ("indice du joueur à entrainer : ");
  //~ scanf ("%d",&userID);  
  //~ printf ("indice de la capacité à entrainer : ");
  //~ scanf ("%d",&buildingID); 
  //~ trainPlayer(sockfd, userID, buildingID);
  /*
  printf("%s\n", "Test du passage d'une liste de mouvement de 7 joueurs :");
  for(int i = 0; i < 7; ++i){
    printf ("indice du joueur à déplacer : ");
    scanf ("%d",&userID);  
    printf ("indice de la diagonale de la destination : ");
    scanf ("%d",&buildingID); 
    printf ("indice de la ligne de la destination : ");
    scanf ("%d",&price); 
    moves[i][0] = userID;
    moves[i][1] = buildingID;
    moves[i][2] = price;
    printf("%s : %d %d %d\n", "playerID diagDest lineDest", userID, buildingID, price);
    printf("%s : %d %d %d\n", "playerID diagDest lineDest", moves[i][0], moves[i][1], moves[i][2]);
  }
  trainPlayer(sockfd, userID, buildingID);
  
  sendMoves(sockfd, moves);
  */
  displayConnexionMenu();
  scanf("%d",&choice);

  if (choice!=0) {
    printf("\nlogin : ");
    scanf("%s",username);
    printf("password : ");
    scanf("%s",password);

    if (choice==1) {
      sendLoginToServer(sockfd,username,password);

      askForManagerInfos(sockfd);
      askForPlayersList(sockfd);
      getManagersList(sockfd);

      scanf("%s",username);
      printf("%s",username);
      answerMatchProposal(sockfd, 1, 1);
    }



    else if (choice==2) {
      sendNewManagerToServer(sockfd,username,password);
      proposeMatchTo(sockfd, 0);
    //result = serverAnswer
    }

  

  }

  
  return EXIT_SUCCESS;
} 
