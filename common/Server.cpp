#include "Server.h"
#include "User.h"
#include "Defines.h"
/* Options choisies, en vue d'une implémentation KIS et d'une séparation de la logique fonctionnelle et de la comm réseau:
- Comme tout middleware, la comm réseau ne doit pas connaître les formats des messages échangés;
- Ceux reçus des clients comportent une commande (1er octet) suivie d'une zone de longueur variable et de contenu quelconque;
- Ceux envoyés vers les clients ... (TBFilled)
- Lors d'une nouvelle connexion, le serveur crée une instance de la classe User;
- Le serveur, sur base de la commande, appelle la fonction applicative idoine (en général une méthode de la classe User);
- Aucune de ces fonctions n'étant bloquante (pas ou peu d'I/O) une solution multi-threads serait de l'over-kill,
	plus complexe tout en étant beaucoup plus gourmande en ressources CPU;
- Travailler avec un seul thread supprime la nécessité de devoir gérer l'accès concurrent aux données (en mémoire ou sur disque)
	et la communication entre threads (arrêt du serveur par exemple);
- Seul le déroulement d'une vente aux enchères devra être accompagné par un thread de cadencement (un par vente) (thread pooling ?);
- On utilise donc la fonction select();
- On peut gérer le terminal du serveur de la même façon (pour commander l'arrêt du serveur par exemple) : STDIN_FILENO.
*/
Server::Server(int port): port_(port), max_(0) {}
// initialisations dans le constructeur
void Server::run() {
	std::cout<<"Server started to listen on port "<<port_<<std::endl;
	if (connect() == EXIT_SUCCESS) {
		mainLoop();
		disconnect();
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
		std::cerr<<"Socket descriptor options initialization error";
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
	std::cout<<"Server listening on port "<<port_<<std::endl;
	sin_size=sizeof(struct sockaddr_in);
	return EXIT_SUCCESS;
}

void Server::disconnect() {
	close(sockfd_);
}

int Server::mainLoop() {
	max_=sockfd_;
	while(true) {
		loadFDSet();
		if (select(max_+1,&FDSet_,NULL,NULL,NULL)==ERROR) {
			std::cerr<<"Socket bind error";
			return EXIT_FAILURE;
		}
		if(keyboard())
		//ajouter ici une gestion de commandes pour le serveur
		//pour l'instant, tout caractère provoque son arrêt
			break;
		if(isNewConnection())
			if(newUser()==EXIT_FAILURE) {
				std::cerr<<"User creation error";
				return EXIT_FAILURE;
			}
		else {
			for (int i=0;i<usersList_.size();++i) {
				if (FD_ISSET(usersList_[i]->getSockfd(),&FDSet_)) {
					if(receive(usersList_[i],msg)==EXIT_SUCCESS) {
						if(usersList_[i]->isDisconnecting()) {
							std::cout<<usersList_[i]->getUserId()<<" disappeared"<<std::endl;
							removeUser(i);
						}
						else std::cout<<msg;
						//ajouter ici le traitement du message
					}
					break;
				}
			}
		}
	}
	removeAllUsers();
	return EXIT_SUCCESS;
}

void Server::loadFDSet() {
//charge le set des FD que doit surveiller en lecture accept()
//comme accept() écrase cet input par la liste des FD actifs,
//il faut exécuter cette fonction avant chaque accept()
	FD_ZERO(&FDSet_); //l'ensemble est vidé
	FD_SET(sockfd_,&FDSet_); //le socket du serveur pour accepter de nouvelles connexions
	FD_SET(STDIN_FILENO,&FDSet_); //le file descriptor de l'input pour accepter des commandes du clavier du serveur
	for (int i=0;i<usersList_.size();++i)
		FD_SET(usersList_[i]->getSockfd(),&FDSet_);
}

bool Server::isNewConnection() {return FD_ISSET(sockfd_,&FDSet_);}

bool Server::keyboard() {return FD_ISSET(STDIN_FILENO,&FDSet_);}

bool Server::newUser() {
	clientSockfd_=accept(sockfd_,(struct sockaddr*)&clientSockAddress_,&sin_size);
	if (clientSockfd_ == ERROR) {
		std::cerr<<"Accept error";
		return EXIT_FAILURE;
	}
	User * user = new User(clientSockfd_);
	usersList_.push_back(user);
	FD_SET(clientSockfd_,&FDSet_);
	if(clientSockfd_>max_) max_= clientSockfd_;
	return EXIT_SUCCESS;
}

int Server::receive(User * aUser, char * buf) {
	int length=recv(aUser->getSockfd(),buf,INPUTSIZE-1,0);
	if(length==ERROR) {
		std::cerr<<aUser->getUserId()<<" message receive error";
		return EXIT_FAILURE;
	}
	else if(length==0) aUser->setDisconnection();
			buf[INPUTSIZE]='\0';
	return EXIT_SUCCESS;
}

int Server::sendToClient(User * aUser, std::string buf) {
	if(send(aUser->getSockfd(),buf.c_str(),buf.size(),0)==ERROR) {
		std::cerr<<aUser->getUserId()<<" message send error";
		return EXIT_FAILURE;
	}
	else return EXIT_SUCCESS;
}

void Server::removeUser(int pos) {
	close(usersList_[pos]->getSockfd());
	usersList_.erase(usersList_.begin()+pos);
}

void Server::removeAllUsers() {
	for(int i=0;i<usersList_.size();++i) removeUser(i);
}
