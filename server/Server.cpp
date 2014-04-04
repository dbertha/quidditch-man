#include "Server.hpp"

// Documentation : voir Readme.txt
Server::Server(int port): __matchesHandler(), port_(port), max_(0), _nextuserID(0) {}
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
std::vector<Auction*> Server::getAuctionsList() {return auctionsList_;}
int Server::getAuctionID(int index) {return auctionsList_[index]->getAuctionID();}
std::vector<int> Server::getPlayerSoldInfos(int index) {return auctionsList_[index]->getPlayerInfos();}
std::string Server::getPlayerSoldName(int index) {return auctionsList_[index]->getPlayerName();}
int Server::getAuctionTimeLeft(int index) {return auctionsList_[index]->getTimeBeforeFirstTurn();}
int Server::getAuctionStartingPrice(int index) {return auctionsList_[index]->getStartingPrice();}

void Server::createAuction(User* user, ManagedPlayer player, int startingPrice) {
	auctionsList_.push_back(new Auction(user,player,startingPrice,auctionsList_.size()));
}

int Server::connect() {
	//initialisation du socket et mise sur écoute :
	sockfd_=buildConnection (port_);

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
    //SerializedObject received;
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
					//int length = receive(usersList_[i],&received); //TODO : tester valeur
					if(usersList_[i]->isDisconnecting()) {
						std::cout<<"User "<<usersList_[i]->getUserID()<<" disappeared from socket "
										<<usersList_[i]->getSockfd()<<std::endl;
						removeUser(i);
					}
					else //traitement du message et envoi de la réponse
						usersList_[i]->cmdHandler();
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
	for (unsigned int i=0;i<usersList_.size();++i){
		FD_SET(usersList_[i]->getSockfd(),&FDSet_);
	}
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
	User * user = new User(this,&__matchesHandler,clientSockfd_, _nextuserID);
	++_nextuserID;
	usersList_.push_back(user);
	FD_SET(clientSockfd_,&FDSet_);
	if(clientSockfd_>max_) max_= clientSockfd_;
	return EXIT_SUCCESS;
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
//int Server::getNewUserId() {return ++userId_;}
