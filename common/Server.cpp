#include "Server.h"
#include "Defines.h"

Server::Server(int port): port_(port), max_(0) {}
// initialisations dans le constructeur
void Server::run() {
	std::cout<<"Server started to listen on port "<<port_<<std::endl;
	if (Server::connect() == EXIT_SUCCESS) {
//		Server::mainLoop;
		Server::disconnect();
	}
	else std::cerr<<"Stack connection aborted"<<std::endl;
}

int Server::connect() {
	//initialisation du socket et mise sur écoute :
	sockfd_=socket(PF_INET,SOCK_STREAM,0);
	if (sockfd_ == ERROR) {
		std::cerr<<"Socket descriptor initialization error";
		return EXIT_FAILURE;
	}
	int yes=1; //autorise deux binds successifs
	if (setsockopt(sockfd_, SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==ERROR) {
		std::cerr<<"Socket descriptor option initialization error";
		return EXIT_FAILURE;
	}
	sockAddress_.sin_family=AF_INET;
	sockAddress_.sin_port=htons(port_);
	sockAddress_.sin_addr.s_addr=INADDR_ANY;
	memset(&sockAddress_.sin_zero,'\0',8);

	if (bind(sockfd_,(struct sockaddr*)&sockAddress_,sizeof(struct sockaddr))==ERROR) {
		std::cerr<<"Socket bind error";
		return EXIT_FAILURE;
	}

	//attente des clients :
	if (listen(sockfd_,BACKLOG)==ERROR) {
		std::cerr<<"listen initialization error";
		return EXIT_FAILURE;
	}
	else return EXIT_SUCCESS;
}

void Server::disconnect() {
	close(sockfd_);
}
