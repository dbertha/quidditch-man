#include "../common/NetworkBase.h"
#include "../common/NetworkInterface.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
  SerializedObject toSendToClient, received;
  int sockfd, new_fd;
  
        // listen on sock_fd, new connection on new_fd
  struct sockaddr_in my_addr;    
        // my address information
  struct sockaddr_in their_addr; 
        // connector's address information
  unsigned int sin_size;
  int yes=1;
  
  if ((sockfd = socket(PF_INET, 
		       SOCK_STREAM, 0)) == -1) {
    perror("Serveur: socket");
    return EXIT_FAILURE;
  }
  
  if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,
		 &yes,sizeof(int)) == -1) {
    perror("Serveur: setsockopt");
    return EXIT_FAILURE;
  }
        

  my_addr.sin_family = AF_INET;         
  my_addr.sin_port = htons(PORT);     
  my_addr.sin_addr.s_addr = INADDR_ANY; 
  memset(&(my_addr.sin_zero), '\0', 8); 

  


  if (bind(sockfd, (struct sockaddr *)&my_addr,
	   sizeof(struct sockaddr)) == -1) {
    perror("Serveur: bind");
    return EXIT_FAILURE;
  }
  
  if (listen(sockfd, BACKLOG) == -1) {
    perror("Serveur: listen");
    return EXIT_FAILURE;
  }
    
  sin_size = sizeof(struct sockaddr_in);
  


  
  while(1){
    
    new_fd = accept(sockfd, 
		    (struct sockaddr *)&their_addr,
		    &sin_size);

    if (new_fd == -1) {
      perror("Serveur: accept");
    }
    
    printf("Serveur:  connection recue du client %s\n",
	   inet_ntoa(their_addr.sin_addr));
    
    if (fork()== 0) {  
      int att1 = 55;
      int att2 = 12750;
      long att3 = 18956078;
      char att4[43] = "Bonjour cher client ! Comment allez-vous ?";
      char username[30], password[30];
      char * position;
      /* this is the child process */
      //~ printf("%s\n", "Contenu du buffer passé sur le réseau :");
      //~ toSendToClient.typeOfInfos = 1;
      //~ printf("%s : %d\n", "type of infos", toSendToClient.typeOfInfos);
      //~ toSendToClient.typeOfInfos = htons(toSendToClient.typeOfInfos);
      //~ toSendToClient.senderId = 111;
      //~ printf("%s : %d\n", "sender ID", toSendToClient.senderId);
      //~ toSendToClient.senderId = htons(toSendToClient.senderId);
      //~ 
      //~ toSendToClient.numInSequence = 1;
      //~ printf("%s : %d\n", "num in sequence", toSendToClient.numInSequence);
      //~ toSendToClient.numInSequence = htons(toSendToClient.numInSequence);
      //~ toSendToClient.nbOfFollowing = 0;
      //~ printf("%s : %d\n", "num of following", toSendToClient.nbOfFollowing);
      //~ toSendToClient.nbOfFollowing = htons(toSendToClient.nbOfFollowing);
      //~ printf("%s\n", "attribut séquentialisés : ");
      //~ printf("%s : %d\n", "Entier 1 ", att1);
      //~ printf("%s : %d\n", "Entier 2 ", att2);
      //~ printf("%s : %d\n", "Long ", att3);
      //~ printf("%s : %s\n", "char[43] :  ", att4);
      //~ position = toSendToClient.stringData;
      //~ memcpy(position, &att1, sizeof(att1));
      //~ position += sizeof(att1);
      //~ memcpy(position, &att2, sizeof(att2));
      //~ position += sizeof(att2);
      //~ memcpy(position, &att3, sizeof(att3));
      //~ position += sizeof(att3);
      //~ printf("%s : %d\n", "Taille du char[] : ", sizeof(att4));
      //~ memcpy(position, &att4, sizeof(att4));
      //~ 
      //~ if(send(new_fd, &toSendToClient, sizeof(SerializedObject), 0) != sizeof(SerializedObject)){
        //~ printf("%s\n", "Erreur dans l'envoi des données");
        //~ return ERROR;
      //~ }
      
      
      printf("%s\n", "Test du passage des infos de logging");
      received = receiveOnSocket(new_fd);
      if(received.typeOfInfos == LOGIN){
        printf("%s\n", "Tentative de login reçue");
        position = received.stringData;
        memcpy(&username, position, sizeof(username)); //sizeof(char array[10]) = 10
        position += sizeof(username);
        memcpy(&password, position, sizeof(password));
        printf("User : %s\n", username);
        printf("Password : %s\n", password);
      }
        
      
      

      printf("%s\n", "Fin de la connexion.");
      return EXIT_SUCCESS;
    }
    
    close(new_fd);  
  }
  
  

  return EXIT_SUCCESS;
}
