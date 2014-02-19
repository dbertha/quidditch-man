#include "Server.hpp"
#include "CommonMgr.hpp"
#include "User.hpp"
#include "commAPI.hpp"
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
					int length = receive(usersList_[i],msg,sizeof(msg));
					if(usersList_[i]->isDisconnecting()) {
						std::cout<<"User "<<usersList_[i]->getUserId()<<" disappeared from socket "
										<<usersList_[i]->getSockfd()<<std::endl;
						removeUser(i);
					}
					else //traitement du message et envoi de la réponse
						usersList_[i]->cmdHandler(msg,length);
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
		if(usersList_[i]->state_!=User::MATCH_INGAME)
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

int Server::receive(User * aUser, char * buf, const int len) {
	int length=recv(aUser->getSockfd(),buf,len,0);
	if(length==0 || length==ERROR) {
		aUser->setDisconnection();
	}
	else {
		buf[length]='\0';
#ifdef __DEBUG
		std::cout<<"**got "<<length<<" char. on "<<clientSockfd_<<" : "<<buf<<"**"<<std::endl;
#endif
	}
	return length;
}

int Server::sendToClient(User * aUser, char * buf, const int length) {
	return sendMsg(aUser->getSockfd(),buf,length);
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