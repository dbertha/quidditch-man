#include "Client.hpp"
#include "commAPI.hpp"
// Documentation : voir Readme.txt
#define PORT 3495
#define ERROR -1
#define INPUTSIZE 200 //taille maximale du message envoyé du client vers le serveur

Client::Client(int sockfd): sockfd_(sockfd), state_(INIT) {}
// initialisations dans le constructeur
void Client::run() {
		mainLoop();
		close(sockfd_);
}

int Client::mainLoop() {
	while(state_!=DISCONNECTING) {
		askInput();
		loadFDSet();
		if (select(sockfd_+1,&FDSet_,NULL,NULL,NULL)==ERROR) {
			std::cerr<<"Socket bind error"<<std::endl;
			return EXIT_FAILURE;
		}
		if(keyboard()) kbMgr();
		else commMgr();
	}
	return EXIT_SUCCESS;
}

void Client::askInput() {
	switch(state_) {
		case INIT : {
			std::cout<<"Bonjour : veuillez svp vous identifier : "<<std::endl;
			break;
		}
		case FREE : {
			displayMenu();
			break;
		}
		default : {
			std::cout<<"Aucune option pour l'instant."<<std::endl;
		}
	}
}

void Client::loadFDSet() {
//charge le set des FD que doit surveiller en lecture accept()
//comme select() écrase cet input par la liste des FD actifs,
//on exécute cette fonction avant chaque select()
	FD_ZERO(&FDSet_); //l'ensemble est vidé
	FD_SET(sockfd_,&FDSet_);
	FD_SET(STDIN_FILENO,&FDSet_); //le file descriptor de l'input pour accepter des commandes du clavier
}

bool Client::keyboard() {return FD_ISSET(STDIN_FILENO,&FDSet_);}

void Client::login() {//à ce stade du projet, pas d'inscription préalable
	msg[0]='L';
	msg[1]=' ';
	strcpy(&msg[2],input_);
	contactServer();
	state_=FREE;
}

void Client::displayMenu() {
	std::cout<<"Que souhaitez-vous faire ?"<<std::endl;
	std::cout<<"Liste des managers disponibles pour un match amical"<<std::endl;
	std::cout<<"Gestion de vos bâtiments"<<std::endl;
	std::cout<<"Consultation des enchères"<<std::endl;
	std::cout<<"Mise aux enchères d'un joueur"<<std::endl;
	std::cout<<"Arrêt du programme"<<std::endl;
	std::cout<<"Introduisez la première lettre de l'option : "<<std::endl;
	msg[0]='F';
	sendMsg(sockfd_,msg,strlen(msg));
}

void Client::kbMgr() {
	std::cin>>input_;
	switch(state_) {
		case INIT : {
			login();
			break;
		}
		case FREE : {
			std::toupper(input_[0]);
			switch (input_[0]) {
				case 'L' : {//Liste des managers disponibles pour un match amical
					matchTentative();
					break;
				}
				case 'G' : {//Gestion de vos bâtiments
					break;
				}
				case 'C' : {//Consultation des enchères
					break;
				}
				case 'M' : {//Mise aux enchères d'un joueur
					break;
				}
				case 'A' : {//Arrêt du programme
					std::cout<<"Au revoir."<<std::endl;
					state_=DISCONNECTING;
					return;
				}
				default : {
					std::cout<<"Commande non reconnue."<<std::endl;
				}
			}
			break;
		}
		default : {//ne devrait jamais passer par ici
			std::cout<<"Je ne puis rien faire pour vous."<<std::endl;
		}
	}
}

void Client::commMgr() {
//gère les messages non sollicités (exemple : invitation à un match amical)
	receiveMessage();
	switch(msg[0]) {
		case 'M' : {
			std::cout<<"Etes-vous d'accord ? (O/N)"<<std::endl;
			std::cin>>input_;
			char cmd=input_[0];
			std::toupper(cmd);
			if(cmd=='O') {
				msg[1]='+';
				sendMsg(sockfd_,msg,strlen(msg));
				state_=MATCH_INGAME;
				const char space[]=" ";
				strncpy(opponent_,&msg[2],strcspn(&msg[2],space));
				std::cout<<"C'est parti pour un match amical !"<<std::endl;
//				startGame();
				state_=FREE;
				return;
			}
			else msg[1]='-';
			break;
		}
		default : {//ne devrait jamais passer par ici
			msg[1]='0';
			strcpy(&msg[2],"Commande non reconnue.");
		}
	}
	sendMsg(sockfd_,msg,strlen(msg));
}

void Client::matchTentative() {
	msg[0]='M';
	msg[1]=' ';
	std::cout<<"Managers disponibles :"<<std::endl;
	contactServer();
	if(msg[1]==' ') {//il y a des managers disponibles
		state_=MATCH_INVITING;
		std::cin>>input_; //nom du manager choisi comme adversaire
		msg[1]='S';
		strcpy(opponent_,input_);
		strcpy(&msg[2],opponent_);
		contactServer();
		if(msg[1]==' ') //le match peut commencer
			state_=MATCH_INGAME;
			std::cout<<"C'est parti pour un match amical !"<<std::endl;
// le nom de mon adversaire se trouve dans opponent_
//			startGame()
		state_=FREE;
	}
}

void Client::contactServer() {
	sendMsg(sockfd_,msg,strlen(msg));
	receiveMessage();
}
void Client::receiveMessage() {
	receiveMsg(sockfd_,msg,INPUTSIZE);
	std::cout<<&msg[2]<<std::endl;
}
