#include "sendToServer.h"
#include "defines.h"

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

int sendTo(const int sockfd, char * msg, const int length) {

	if (send(sockfd,msg,length,0) == ERROR) {
		perror("Client: send");
		return EXIT_FAILURE;
	}
#ifdef __DEBUG
	printf("**envoi : %s\n",msg);
#endif
	return EXIT_SUCCESS;
}

int receiveFrom(const int sockfd, char * msg, const int len) {
	int length;
	length=recv(sockfd,msg,len,0);
	if (length < 0) perror("Client: receive");
	else {
		if (length==0) printf("Le serveur est a l'arret\n");
		else {
			msg[length]='\0'; //insert une End of String
#ifdef __DEBUG
			printf("**reçu : %s\n",msg);
#endif
		}
	}
	return length;
}
