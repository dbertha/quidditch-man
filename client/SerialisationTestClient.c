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

#define DIM 100

int main(int argc, char *argv[]){
  int buildingID;
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
  
  printf("%s\n", "Test du passage des infos de logging :");
  printf ("Enter your username : (no spaces allowed)");
  scanf ("%29s",username);  
  printf ("Enter your password : ");
  scanf ("%29s",password);  
  
  sendLoginToServer(sockfd, username, password);
  
  printf("%s\n", "Test du passage des infos de bâtiments :");
  printf ("indice du bâtiment demandé : ");
  scanf ("%d",&buildingID);  
  
  askForBuildingInfos(sockfd, buildingID);
  
  printf("%s\n", "Test du passage d'une demande d'upgrade de bâtiments :");
  printf ("indice du bâtiment demandé : ");
  scanf ("%d",&buildingID);  
  
  askForBuildingUpgrade(sockfd, buildingID);
  
  
  return EXIT_SUCCESS;
} 
