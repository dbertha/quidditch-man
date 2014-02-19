#include "Client.hpp"
#include "commAPI.hpp"
#define PORT 3495
#define ERROR -1

int main(int argc, char* argv[]) {
	int sockfd_;
	struct sockaddr_in serverAddr;
	struct hostent *he; // Pointeur vers le descripteur IP du serveur

	std::cout<<"Bienvenue cher client."<<std::endl;

	// Initialisation des paramètres de connection au serveur
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT); // Le port du serveur.
	memset(&(serverAddr.sin_zero), '\0', 8);
	switch (argc) {
		case 2 : if ((he=gethostbyname(argv[1])) == NULL) { // Pointeur vers le descripteur IP du serveur a partir du nom passe en parametre.
				std::cout<<"La machine "<<argv[1]<<" n'a pas ete trouvee"<<std::endl;
				return EXIT_FAILURE;
			}
			serverAddr.sin_addr = *((struct in_addr*)he->h_addr);
			std::cout<<"Adresse du serveur "<<argv[1]<<" : "<<inet_ntoa(serverAddr.sin_addr)<<std::endl;
			break;
		case 3 :if (inet_aton(argv[2],&serverAddr.sin_addr) != 0) break;
		default : std::cout<<"Donner le nom de la machine distante en argument."<<std::endl	\
					<<"ou son adresse IP sous la forme <IP adresse>. Exemples :"<<std::endl		\
					<<"client nom-machine-serveur"<<std::endl<<"client IP 192.168.1.6"<<std::endl;
				return EXIT_FAILURE;
	}
	sockfd_ = socket(PF_INET,SOCK_STREAM, 0);
	if (sockfd_ == ERROR) return EXIT_FAILURE;
	if (connect(sockfd_, (struct sockaddr *)&serverAddr,sizeof(struct sockaddr)) == EXIT_FAILURE) {
		std::cout<<"Le serveur est absent."<<std::endl;
		return EXIT_FAILURE;
	}

	Client ourClient=Client(sockfd_);
	ourClient.run();
	return EXIT_SUCCESS;
}
