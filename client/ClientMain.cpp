#include "TerminalGame.hpp"

//TODO : utiliser buildConnexion de NetworkBase
int main(int argc, char* argv[]) {
	int sockfd_;
	struct sockaddr_in serverAddr;
	struct hostent *he; // Pointeur vers le descripteur IP du serveur
	char serverName[60];
	std::cout<<"Bienvenue cher client."<<std::endl;
	  
  
	if (argc != 2) {
		if(argc == 1){ /*pas d'arguments : serveur est sur la machine locale */
			gethostname(serverName, 60);
			std::cout << "Le server est la machine locale, dont le nom est " << serverName << std::endl;
			if ((he=gethostbyname(serverName)) == NULL) { 
				std::cerr << "Client: gethostbyname" << std::endl;
				return EXIT_FAILURE;
			}
			serverAddr.sin_addr = *((struct in_addr*)he->h_addr);
		}else{
			if(inet_aton(argv[2],&serverAddr.sin_addr) != 0){
				std::cerr <<"Donner le nom de la machine distante en argument."<<std::endl	\
					<<"ou son adresse IP sous la forme <IP adresse>. Exemples :"<<std::endl		\
					<<"client nom-machine-serveur"<<std::endl<<"client IP 192.168.1.6"<<std::endl;
				return EXIT_FAILURE;
			}
		}
	}else{
		int i = 0;
		while((argv[i] != '\0') && (i < DIM)){
			serverName[i] = argv[1][i];
			++i;
		}
		serverName[i] = '\0';
		std::cout << "Le nom du server distant est " << serverName <<std::endl;
		if ((he=gethostbyname(serverName)) == NULL) { 
			perror("Client: gethostbyname");
			return EXIT_FAILURE;
		}
		serverAddr.sin_addr = *((struct in_addr*)he->h_addr);
	}
	
	// Initialisation des paramètres de connection au serveur
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT); // Le port du serveur.
	memset(&(serverAddr.sin_zero), '\0', 8);
	sockfd_ = socket(PF_INET,SOCK_STREAM, 0);
	if (sockfd_ == ERROR) return EXIT_FAILURE;
	if (connect(sockfd_, (struct sockaddr *)&serverAddr,sizeof(struct sockaddr)) == EXIT_FAILURE) {
		std::cout<<"Le serveur est absent."<<std::endl;
		return EXIT_FAILURE;
	}
	TerminalGame game(sockfd_);
	game.mainLoop();
	//~ Client ourClient=Client(sockfd_);
	//~ ourClient.run();
	close(sockfd_);
	return EXIT_SUCCESS;
}
