#include "Client.hpp"
#include "commAPI.hpp"
// Documentation : voir Readme.txt


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

///////Display\\\\\\\

//Main\\

void Client::displayManagerInfos() {
    int nbPlayers, money, nbFans;
    askForManagerInfos(sockfd_);
    receiveManagerInfos(sockfd_,&nbPlayers,&money,&nbFans);
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<" -------------------- MONEY : "<<money<<" ||| "<<" NUMBER OF FANS : "<<nbFans<<" --------------------"<<endl;
}

void Client::displayMainMenu(){
  cout<<" --------------------------------------------------------------------------------------"<<endl;
  cout<<"What do you want to do ?"<<endl;
  cout<<" [1] See managers connected to propose match"<<endl;
  cout<<" [2] See/create auctions"<<endl;
  cout<<" [3] Manage players"<<endl;
  cout<<" [4] Manage buildings"<<endl;
  cout<<" [0] Quit game"<<endl;
  cout<<"-----> ";
}

//Log in\\

void Client::displayConnexionMenu(){
  cout<<" ------------------------ WELCOME TO QUIDDITCH MANAGER 2014 ! ------------------------"<<endl;
  cout<<" [1] Log in"<<endl;;
  cout<<" [2] Create a new manager"<<endl;
  cout<<"-----> ";
}

//Management\\

void Client::displayManagePlayersMenu(){
  cout<<" --------------------------------------------------------------------------------------"<<endl;
  cout<<"//MANAGE TEAM// What do you want to do ?"<<endl;
  cout<<" [1] Inspect a player"<<endl;
  cout<<" [2] Train a player"<<endl;
  cout<<" [3] Send a player to the hospital"<<endl;
  cout<<" [0] Return to the main menu"<<endl;
  cout<<"-----> ";
}

void Client::displayManageBuildingsMenu(){
  cout<<" --------------------------------------------------------------------------------------"<<endl;
  cout<<"//MANAGE BUILDINGS// What do you want to do ?"<<endl;
  cout<<" [1] Enter Stadium"<<endl;
  cout<<" [2] Enter Training Center"<<endl;
  cout<<" [3] Enter Hospital"<<endl;
  cout<<" [4] Enter FanShop"<<endl;
  cout<<" [0] Return to the main menu"<<endl;
  cout<<"-----> ";
}

void Client::displayPlayersList() {
  askForPlayersList(sockfd_);
  vector<string> playersList = receivePlayersList(sockfd_);
  for (int i=0;i<playersList.size();i+=2) {cout<<" ["<<(i+2)/2<<"] "<<playersList[i]<<" "<<playersList[i+1]<<endl;}
}

void Client::displayBuildingInfos(std::vector<int> buildingInfos, int buildingID){
  cout<<"Current level : "<<buildingInfos[0]<<endl;
  cout<<"Price for next level : "<<buildingInfos[1]<<endl;
  if (buildingID==STADIUM) cout<<"Max places in the Stadium : "<<buildingInfos[2]<<endl;
  else if (buildingID==TRAININGCENTER) cout<<"Time required to finish training : "<<buildingInfos[2]<<endl;
  else if (buildingID==HOSPITAL) cout<<"Time required to finish healing : "<<buildingInfos[2]<<endl;
  else if (buildingID==FANSHOP) cout<<"Max clients in the fan shop per match : "<<buildingInfos[2]<<endl;
  if (buildingInfos[3]==1) cout<<"This building is currently being upgraded"<<endl;
  else cout<<"This building is not upgrading yet.";
}

void displayPlayerInfos(vector<int> playerInfos, int playerID) {
  askForPlayersList(sockfd_);
  std::vector<string> playersList = receivePlayersList(sockfd_);
  cout<<"-------- "<<playersList[2*playerID]<<" "<<playersList[2*playerID+1]<<" --------"<<endl;
  cout<<" [1] Speed : "<<playerInfos[0]<<endl;
  cout<<" [2] Strength : "<<playerInfos[1]<<endl;
  cout<<" [3] Precision : "<<playerInfos[2]<<endl;
  cout<<" [4] Reflex : "<<playerInfos[3]<<endl;
  cout<<" [5] Resistance : "<<playerInfos[4]<<endl;
  cout<<"---------------------------------\n Number of trainings left to up speed : "<<playerInfos[5]<<endl;
  cout<<" Number of trainings left to up strength : "<<playerInfos[6]<<endl;
  cout<<" Number of trainings left to up precision : "<<playerInfos[7]<<endl;
  cout<<" Number of trainings left to up reflex : "<<playerInfos[8]<<endl;
  cout<<" Number of trainings left to up resistance : "<<playerInfos[9]<<endl;
  if (playerInfos[10]==1) cout<<"\n This player is currently blocked."<<endl;
  else cout<<"\n This player isn't blocked."<<endl;
  if (playerInfos[12]>0) {
    cout<<" The broomstick of this player grants a bonus of "<<playerInfos[12]<<" for ";
    if (playerInfos[11]==0) cout<<"speed"<<endl;
    else if (playerInfos[11]==1) cout<<"strength"<<endl;
    else if (playerInfos[11]==2) cout<<"precision"<<endl;
    else if (playerInfos[11]==3) cout<<"reflexx"<<endl;
    else if (playerInfos[11]==4) cout<<"resistance"<<endl;
  }
  else cout<<" The broomstick of this player doesn't grant him any bonus."<<endl;

  cout<<" The life of this player is "<<playerInfos[13]<<" (max="<<playerInfos[4]<<")"<<endl;
  cout<<" The value of this player is estimated at "<<playerInfos[14]<<" gold"<<endl;
}

//Auctions\\

void Client::displayAuctionMenus(){
  cout<<" --------------------------------------------------------------------------------------"<<endl;
  cout<<"//YOU'RE NOW IN THE AUCTION ROOM// What do you want to do ?"<<endl;
  cout<<" [1] See current auctions"<<endl;
  cout<<" [2] Sell a player"<<endl;
  cout<<" [0] Return to the main menu"<<endl;
  cout<<"-----> ";
}

void Client::displaySellPlayerMenu(){
  cout<<" --------------------------------------------------------------------------------------"<<endl;
  cout<<"//YOU'RE ABOUT TO SELL A PLAYER// What do you want to do ?"<<endl;
  displayPlayersList(sockfd_);
  cout<<" Which player do you want to sell [enter 0 to abort] ? "<<endl;
}


std::string Client::displayAuctionInfos(vector<string> auctionsList,vector<int> playerInfos, int auctionID) {
  std::string name;
  std::string price;
  std::string timeLeft;
  for (int i=0;i<auctionsList.size();i+=4) {
    if (atoi(auctionsList[i].c_str())==auctionID) {
      name = auctionsList[i+1];
      price = auctionsList[i+3];
      timeLeft = auctionsList[i+2];
    }
  }
  cout<<"-------- "<<name<<" --------"<<endl;
  cout<<" [1] Speed : "<<playerInfos[0]<<endl;
  cout<<" [2] Strength : "<<playerInfos[1]<<endl;
  cout<<" [3] Precision : "<<playerInfos[2]<<endl;
  cout<<" [4] Reflex : "<<playerInfos[3]<<endl;
  cout<<" [5] Resistance : "<<playerInfos[4]<<endl;
  cout<<"---------------------------------\n Number of trainings left to up speed : "<<playerInfos[5]<<endl;
  cout<<" Number of trainings left to up strength : "<<playerInfos[6]<<endl;
  cout<<" Number of trainings left to up precision : "<<playerInfos[7]<<endl;
  cout<<" Number of trainings left to up reflex : "<<playerInfos[8]<<endl;
  cout<<" Number of trainings left to up resistance : "<<playerInfos[9]<<endl;
  if (playerInfos[10]==1) cout<<"\n This player is currently blocked."<<endl;
  else cout<<"\n This player isn't blocked."<<endl;
  if (playerInfos[12]>0) {
    cout<<" The broomstick of this player grants a bonus of "<<playerInfos[12]<<" for ";
    if (playerInfos[11]==0) cout<<"speed"<<endl;
    else if (playerInfos[11]==1) cout<<"strength"<<endl;
    else if (playerInfos[11]==2) cout<<"precision"<<endl;
    else if (playerInfos[11]==3) cout<<"reflex"<<endl;
    else if (playerInfos[11]==4) cout<<"resistance"<<endl;
  }
  else cout<<" The broomstick of this player doesn't grant him any bonus."<<endl;

  cout<<" The life of this player is "<<playerInfos[13]<<" (max="<<playerInfos[4]<<")"<<endl;
  cout<<" The value of this player is estimated at "<<playerInfos[14]<<" gold"<<endl;
  cout<<"\n STARTING PRICE FOR THE AUCTION : "<<price<<endl;

  return timeLeft;
}

void displayAuctionsList(vector<string> auctionsList) {
  if (auctionsList.size()==0) cout<<" --------- No auction available"<<endl;
  else {
    for (int i=0;i<auctionsList.size();i+=4) {
      cout<<" ["<<atoi(auctionsList[i].c_str())+1<<"] "<<auctionsList[i+1]<<" | Starting price : "<<auctionsList[i+3];
      cout<<" | Time left to participate : "<<auctionsList[2]<<endl;
    }
    cout<<"Indicate which auction you wish to inspect [or 0 to abort] : ";
  }
}



//Match\\

void Client::askInput() {
	switch(state_) {
		case INIT : {
			displayConnexionMenu();
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
	FD_SET(sockfd_,&FDSet_); //le socket
	FD_SET(STDIN_FILENO,&FDSet_); //le file descriptor de l'input pour accepter des commandes du clavier
}

bool Client::keyboard() {return FD_ISSET(STDIN_FILENO,&FDSet_);}

void Client::login() {
	contactServer();
	state_=FREE;
}



//~ void Client::displayMenu() {
	//~ std::cout<<"Que souhaitez-vous faire ?"<<std::endl;
	//~ std::cout<<"Liste des managers disponibles pour un match amical"<<std::endl;
	//~ std::cout<<"Gestion de vos bâtiments"<<std::endl;
	//~ std::cout<<"Consultation des enchères"<<std::endl;
	//~ std::cout<<"Mise aux enchères d'un joueur"<<std::endl;
	//~ std::cout<<"Arrêt du programme"<<std::endl;
//~ 
//~ }

void Client::kbMgr() {
	std::cin>>input_;
	switch(state_) {
		case INIT : {
			login();
			break;
		}
		case FREE : {
            
            //affichage du sous-menu avec adaptation du contexte
            
			//~ std::toupper(input_[0]);
			//~ switch (input_[0]) {
				//~ case 'L' : {//Liste des managers disponibles pour un match amical
					//~ matchTentative();
					//~ break;
				//~ }
				//~ case 'G' : {//Gestion de vos bâtiments
					//~ break;
				//~ }
				//~ case 'C' : {//Consultation des enchères
					//~ break;
				//~ }
				//~ case 'M' : {//Mise aux enchères d'un joueur
					//~ break;
				//~ }
				//~ case 'A' : {//Arrêt du programme
					//~ std::cout<<"Au revoir."<<std::endl;
					//~ state_=DISCONNECTING;
					//~ return;
				//~ }
				//~ default : {
					//~ std::cout<<"Commande non reconnue."<<std::endl;
				//~ }
			//~ }
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
