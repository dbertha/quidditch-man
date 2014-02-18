#include "Server.hpp"
#include "CommonMgr.hpp"
#include "User.hpp"
#include <netinet/in.h> //htons()  ntohs()
#include "../common/NetworkBase.h"

//TODO : pas de EXIT_SUCCESS ou EXIT_FAILURE pour d'autres méthodes que main()

// Documentation : voir Readme.txt
Server::Server(int port): commonMgr_(new CommonMgr(this)), port_(port), max_(0) {}
// initialisations dans le constructeur
void Server::run() {
	std::cout<<"Server started to listen on port "<<port_<<std::endl;
	if (connect() == EXIT_SUCCESS) {
		mainLoop();
		disconnect();
	}
	else std::cerr<<"Stack connection aborted"<<std::endl;
}

std::vector<User*> Server::GetUsersList() {return usersList_;}

int Server::connect() {
	//initialisation du socket et mise sur écoute :
	sockfd_=socket(PF_INET,SOCK_STREAM,0);
	if (sockfd_ == ERROR) {
		std::cerr<<"Socket descriptor initialization error"<<std::endl;
		return EXIT_FAILURE;
	}
	int yes=1; //autorise deux binds successifs
	if (setsockopt(sockfd_, SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==ERROR) {
		std::cerr<<"Socket descriptor options initialization error"<<std::endl;
		return EXIT_FAILURE;
	}
	int n;
	unsigned int m = sizeof(n);
	getsockopt(sockfd_,SOL_SOCKET,SO_RCVBUF,(void *)&n, &m);
	std::cout<<"TCP buffer size : "<<n<<std::endl;
	sockAddress_.sin_family=AF_INET;
	sockAddress_.sin_port=htons(port_);
	sockAddress_.sin_addr.s_addr=INADDR_ANY;
	memset(&sockAddress_.sin_zero,'\0',8);

	if (bind(sockfd_,(struct sockaddr*)&sockAddress_,sizeof(struct sockaddr))==ERROR) {
		std::cerr<<"Socket bind error"<<std::endl;
		return EXIT_FAILURE;
	}

	//attente des clients :
	if (listen(sockfd_,BACKLOG)==ERROR) {
		std::cerr<<"listen initialization error"<<std::endl;
		return EXIT_FAILURE;
	}
	std::cout<<"Server listening on port "<<port_<<std::endl;
	sin_size=sizeof(struct sockaddr_in);
	return EXIT_SUCCESS;
}

void Server::disconnect() {
	close(sockfd_);
}

int Server::mainLoop() {
	SerializedObject received;
	max_=sockfd_;
#ifdef __DEBUG
	std::cout<<"**mon socket : "<<sockfd_<<std::endl;
#endif
	while(true) {
		loadFDSet();
		if (select(max_+1,&FDSet_,NULL,NULL,NULL)==ERROR) {
			std::cerr<<"Socket bind error"<<std::endl;
			return EXIT_FAILURE;
		}
		if(keyboard()) {
		//ajouter ici une gestion de commandes pour le serveur
		//pour l'instant, tout Enter provoque son arrêt
			std::cout<<"Server stops"<<std::endl;
			break;
		}
		if(isNewConnection()) {
#ifdef __DEBUG
			std::cout<<"**isNewConnection"<<std::endl;
#endif
			if(newUser()==EXIT_FAILURE) {
				std::cerr<<"User creation error"<<std::endl;
				return EXIT_FAILURE;
			}
		}
		else {
			for (unsigned int i=0;i<usersList_.size();++i) {
				if (FD_ISSET(usersList_[i]->getSockfd(),&FDSet_)) {
					int length = receive(usersList_[i],&received); //TODO : tester valeur
					if(usersList_[i]->isDisconnecting()) {
						std::cout<<"User "<<usersList_[i]->getUserId()<<" disappeared from socket "
										<<usersList_[i]->getSockfd()<<std::endl;
						removeUser(i);
					}
					else //traitement du message et envoi de la réponse
						usersList_[i]->cmdHandler(&received);
				}
			}
		}
	}
	removeAllUsers();
	return EXIT_SUCCESS;
}

void Server::loadFDSet() {
//charge le set des FD que doit surveiller en lecture accept()
//comme select() écrase cet input par la liste des FD actifs,
//on exécute cette fonction avant chaque select()
//cela prend automatiquement en compte les ajouts et les départs
	FD_ZERO(&FDSet_); //l'ensemble est vidé
	FD_SET(sockfd_,&FDSet_); //le socket du serveur pour accepter de nouvelles connexions
	FD_SET(STDIN_FILENO,&FDSet_); //le file descriptor de l'input pour accepter des commandes du clavier du serveur
	for (unsigned int i=0;i<usersList_.size();++i)
		FD_SET(usersList_[i]->getSockfd(),&FDSet_);
}

bool Server::isNewConnection() {return FD_ISSET(sockfd_,&FDSet_);}

bool Server::keyboard() {return FD_ISSET(STDIN_FILENO,&FDSet_);}

int Server::newUser() {
	clientSockfd_=accept(sockfd_,(struct sockaddr*)&clientSockAddress_,&sin_size);
	std::cout<<"**socket du nouveau client : "<<clientSockfd_<<std::endl;
	if (clientSockfd_ == ERROR) {
		std::cerr<<"Accept error"<<std::endl;
		return EXIT_FAILURE;
	}
	User * user = new User(this,commonMgr_,clientSockfd_);
	usersList_.push_back(user);
	FD_SET(clientSockfd_,&FDSet_);
	if(clientSockfd_>max_) max_= clientSockfd_;
	return EXIT_SUCCESS;
}

int Server::receive(User * aUser, SerializedObject * received) {
	int length=recv(aUser->getSockfd(),received,sizeof(SerializedObject),0);
	if(length==0 || length==ERROR) {
		aUser->setDisconnection();
	}
	else {
		received->typeOfInfos = ntohs(received->typeOfInfos);
#ifdef __DEBUG
		std::cout<<"**got a SerializedObject on "<<clientSockfd_<<std::endl;
#endif
	}
	return length;
}

int Server::sendToClient(User * aUser, SerializedObject * toSend) {
	toSend->typeOfInfos = htons(toSend->typeOfInfos);
	if(send(aUser->getSockfd(),toSend,sizeof(SerializedObject),0)==ERROR) {
		std::cerr<<aUser->getUserId()<<" message send error on socket "
				<<aUser->getSockfd()<<std::endl;
		return EXIT_FAILURE;
	}
	else {
#ifdef __DEBUG
		std::cout<<"**sent a SerializedObject to "<<clientSockfd_<<std::endl;
#endif
		return EXIT_SUCCESS;
	}
}

void Server::removeUser(int pos) {
	close(usersList_[pos]->getSockfd());
	usersList_[pos]->setDisconnection();
	usersList_.erase(usersList_.begin()+pos);
}

void Server::removeAllUsers() {
#ifdef __DEBUG
	std::cout<<"**removeAllUsers"<<std::endl;
#endif
	for(unsigned int i=0;i<usersList_.size();++i) removeUser(i);
}
