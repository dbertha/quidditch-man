#include "NetworkBase.h"
#include "Serialisator.hpp"
#include <iostream>

int main(int argc, char *argv[]){
  int sockfd, action, connected;  
  struct sockaddr_in server_addr; 
  struct hostent *server;
  char serverName[DIM];
  
  if (argc != 2) {
    if(argc == 1){ //pas d'arguments : serveur est sur la machine locale
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
  
  if (connect(socketd, (struct sockaddr *) server_addr,
	      sizeof(struct sockaddr)) == -1) {
    perror("Client: connect");
    return 0;
  }
  
  
  
  return EXIT_SUCCESS;
} 
