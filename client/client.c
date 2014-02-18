#include "sendToServer.h"
#include "defines.h"

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
		if(sendTo(sockfd,msg,strlen(msg))==EXIT_FAILURE) break;
		if(receiveFrom(sockfd,msg,sizeof(msg))<=0) break;
		if(strcmp(msg, "disconnect") == 0) stopConnection(sockfd);
	}
	while (strcmp(msg, "disconnect") != 0);

	printf("Au revoir.\n");
	return EXIT_SUCCESS;
}
