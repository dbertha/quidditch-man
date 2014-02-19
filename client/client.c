#include "../common/Defines.hpp"
//#include "../common/commAPI.hpp" //erreur, pb de librairies c++ à passer au compilateur c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int startConnection(const int argc, const char *argv[]) {
	int sockfd;
	struct sockaddr_in serverAddr;
	struct hostent *he; // Pointeur vers le descripteur IP du serveur

	// Initialisation des paramètres de connection au serveur
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT); // Le port du serveur.
	memset(&(serverAddr.sin_zero), '\0', 8);
	switch (argc) {
		case 2 : if ((he=gethostbyname(argv[1])) == NULL) { // Pointeur vers le descripteur IP du serveur a partir du nom passe en parametre.
				perror("Client: gethostbyname");
				printf("La machine %s n'a pas ete trouvee\n",argv[1]);
				return ERROR;
			}
			serverAddr.sin_addr = *((struct in_addr*)he->h_addr);
			printf("Adresse du serveur %s : %s\n",argv[1],inet_ntoa(serverAddr.sin_addr));
			break;
		case 3 :if (inet_aton(argv[2],&serverAddr.sin_addr) != 0) break;
		default : printf("Donner le nom de la machine distante en argument.\n"
				"ou son adresse IP sous la forme <IP adresse>. Exemples :\n"
				"./client nom-machine-serveur\n"
				"./client IP 192.168.1.6\n");
				return ERROR;
	}
	sockfd = socket(PF_INET,SOCK_STREAM, 0);
	if (sockfd == ERROR) {
		perror("Client: socket");
		return ERROR;
	}

	if (connect(sockfd, (struct sockaddr *)&serverAddr,sizeof(struct sockaddr)) == ERROR) {
		perror("Client: connect");
		printf("Le serveur est absent\n");
		return ERROR;
	}
	return sockfd;
}

int stopConnection(const int sockfd) {
	if (close(sockfd) == ERROR) {
		perror("Client: disconnect");
		return EXIT_FAILURE;
	}
	else return EXIT_SUCCESS;
}

void ask_input(char *input) {
	printf("Input: ");
	scanf("%99s",input); // attention un input plus long provoquera un buffer overflow
}

int main(const int argc, const char *argv[]) {
	int sockfd;
	char msg[INPUTSIZE];

	printf("Bienvenue cher client.\n");

	// Initialisation des paramètres de connection au serveur
	sockfd=startConnection(argc,argv);
	if(sockfd==ERROR) return EXIT_FAILURE;
	do {
		ask_input(msg);
		//~ if(sendTo(sockfd,msg,strlen(msg))==EXIT_FAILURE) break;
		//~ if(receiveFrom(sockfd,msg,sizeof(msg))<=0) break;
		if(strcmp(msg, "disconnect") == 0) stopConnection(sockfd);
	}
	while (strcmp(msg, "disconnect") != 0);

	printf("Au revoir.\n");
	return EXIT_SUCCESS;
}
