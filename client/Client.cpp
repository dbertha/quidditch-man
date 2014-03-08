#include "Client.hpp"
#include "commAPI.hpp"
// Documentation : voir Readme.txt

using namespace std; //TODO : rajouter tous les spécificateurs de namespace

//TODO : solution sans thread ou bien passage du pointeur this au thread
bool hasChosen;
bool isBidder;

Client::Client(int sockfd, bool GUI): GUI_(GUI), sockfd_(sockfd), state_(INIT) {}

void Client::run() {
    if(GUI_){
    }else{
        mainLoop();
        close(sockfd_);
    }
}

int Client::mainLoop() {
    while(state_!=DISCONNECTING) {
        askInput();
        loadFDSet();
        if (select(sockfd_+1,&FDSet_,NULL,NULL,NULL)==ERROR) {
            std::cerr<<"Socket bind error"<<std::endl;
            return EXIT_FAILURE;
        }
        if(keyboard()) kbMgr(); // si entrée clavier
        else commMgr(); //sinon, message du serveur
    }
    return EXIT_SUCCESS;
}

///////Display\\\\\\\

//Main\\

void Client::displayManagerInfos() {
    int nbPlayers, money, nbFans, actionPoints;
    askForManagerInfos();
    receiveManagerInfos(&nbPlayers,&money,&nbFans,&actionPoints);
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<" ------ MONEY : "<<money<<" ||| "<<" NUMBER OF FANS : "<<nbFans<<" ||| "<<" ACTION POINTS : "<<actionPoints<<" ------ "<<endl;
}

void Client::displayMainMenu(){
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<"What do you want to do ?"<<endl;
    cout<<" [1] See managers connected to propose match"<<endl;
    cout<<" [2] See/create auctions"<<endl;
    cout<<" [3] Manage players"<<endl;
    cout<<" [4] Manage buildings"<<endl;
    cout<<" [5] Get action points"<<endl;
    cout<<" [0] Quit game"<<endl;
    cout<<"-----> " << flush;
}

//Log in\\

void Client::displayConnexionMenu(){
    cout<<" ------------------------ WELCOME TO QUIDDITCH MANAGER 2014 ! ------------------------"<<endl;
    cout<<" [1] Log in"<<endl;;
    cout<<" [2] Create a new manager"<<endl;
    cout<<"-----> " << flush;
}

//Management\\

void Client::displayManagePlayersMenu(){
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<"//MANAGE TEAM// What do you want to do ?"<<endl;
    cout<<" [1] Inspect a player"<<endl;
    cout<<" [2] Train a player"<<endl;
    cout<<" [3] Send a player to the hospital"<<endl;
    cout<<" [0] Return to the main menu"<<endl;
    cout<<"-----> " << flush;
}

void Client::displayManageBuildingsMenu(){
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<"//MANAGE BUILDINGS// What do you want to do ?"<<endl;
    cout<<" [1] Enter Stadium"<<endl;
    cout<<" [2] Enter Training Center"<<endl;
    cout<<" [3] Enter Hospital"<<endl;
    cout<<" [4] Enter FanShop"<<endl;
    cout<<" [5] Enter PromotionCenter"<<endl;
    cout<<" [0] Return to the main menu"<<endl;
    cout<<"-----> " << flush;
}

void Client::displayActionPointsMenu(){
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<"//GET ACTION POINTS// What do you want to do ?"<<endl;
    cout<<" [1] Buy action points"<<endl;
    cout<<" [2] Start a promotion campaign"<<endl;
    cout<<" [0] Return to the main menu"<<endl;
    cout<<"-----> " << flush;  
}

void Client::displayPlayersList() {
    askForPlayersList();
    vector<string> playersList = receivePlayersList();
    for (unsigned int i=0;i<playersList.size();i+=2) {cout<<" ["<<(i+2)/2<<"] "<<playersList[i]<<" "<<playersList[i+1]<<endl;}
}

void Client::displayBuildingInfos(std::vector<int> buildingInfos, int buildingID){
    cout<<"Current level : "<<buildingInfos[0]<<endl;
    cout<<"Price for next level : "<<buildingInfos[1]<<endl;
    if (buildingID==STADIUM) cout<<"Max places in the Stadium : "<<buildingInfos[2]<<endl;
    else if (buildingID==TRAININGCENTER) cout<<"Time required to finish training : "<<buildingInfos[2]<<endl;
    else if (buildingID==HOSPITAL) cout<<"Time required to finish healing : "<<buildingInfos[2]<<endl;
    else if (buildingID==FANSHOP) cout<<"Max clients in the fan shop per match : "<<buildingInfos[2]<<endl;
    else if (buildingID==PROMOTIONCENTER) cout<<"Action points gained by waiting "<<TIMESCALEACTIONPOINTS<<" minutes : "<<buildingInfos[2]<<endl;
    if (buildingInfos[3]==1) cout<<"This building is currently being upgraded"<<endl;
    else cout<<"This building is not upgrading yet.";
}

void Client::displayPlayerInfos(vector<int> playerInfos, int playerID) {
    askForPlayersList();
    std::vector<string> playersList = receivePlayersList();
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
    cout<<"-----> " << flush;
}

void Client::displaySellPlayerMenu(){
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<"//YOU'RE ABOUT TO SELL A PLAYER// What do you want to do ?"<<endl;
    displayPlayersList();
    cout<<" ---- Selling a player costs "<<AP_AUCTION<<" action points ----"<<endl;
    cout<<" Which player do you want to sell [enter 0 to abort] ? \n-----> "<<endl;
}

void Client::displayAvailableManagers(){
    getManagersList();
    std::vector<int> IDList;
    std::vector<std::string> namesList;
    receiveManagersIDandNames(&IDList,&namesList);
    for(unsigned int i = 0; i < IDList.size(); ++i){
        std::cout << "ID :" << IDList[i] << " name : " << namesList[i] << std::endl;
    }
    cout<<"Indicate the ID of the player you want to challenge [-1 to go back]: " << flush;
}



std::vector<int> Client::displayAndAskPlayersForMatch(){
  displayPlayersList();
  std::cout << "Veuillez donner les index de vos joueur dans l'ordre suivant : KEEPER SEEKER CHASER1 CHASER2 CHASER3 BEATER1 BEATER2" << std::endl;
  int managedIndex;
  std::vector<int> playersInTeam;
  for(int i = 0; i < 7; ++i){
    cout << "indice joueur : " ;
    cin >> managedIndex;
    playersInTeam.push_back(managedIndex-1); //index commence à 0, affichage commence à 1
  }
  return playersInTeam;
}



std::string Client::displayAuctionInfos(vector<string> auctionsList,vector<int> playerInfos, int auctionID) {
    std::string name;
    std::string price;
    std::string timeLeft;
    for (unsigned int i=0;i<auctionsList.size();i+=4) {
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
        for (unsigned int i=0;i<auctionsList.size();i+=4) {
            cout<<" ["<<atoi(auctionsList[i].c_str())+1<<"] "<<auctionsList[i+1]<<" | Starting price : "<<auctionsList[i+3];
            cout<<" | Time left to participate : "<<auctionsList[2]<<endl;
        }
        cout<<"Indicate which auction you wish to inspect [or 0 to abort] : ";
    }
}



//Match\\

////////handling interactions :\\\\\\\



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

void Client::handleLogin(){
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
    std::cout<<"Enter login : ";
    std::cin>>username;
    std::cout<<"Enter password : ";
    std::cin>>password;
    if (input_==LOG_IN) sendLoginToServer(username,password);
    else if (input_==NEW_MANAGER) sendNewManagerToServer(username,password);
    int result = getConfirmation();
    if (result == LOGIN_FAILED) {
        if (input_==LOG_IN) cout<<" ------------- WRONG LOGIN/PASSWORD"<<endl;
        else if (input_==NEW_MANAGER) cout<<" ------------- LOGIN ALREADY TAKEN"<<endl;
    }
    else{
        if(result == NORMAL_LOGIN){
            state_ = FREE;
        }
        else{
            //ADMIN_LOGIN
            state_ = ADMIN;
        }
    }
}

void Client::handleMainMenu(){
    

    switch(input_){
//Rappel :
//#define SEE_MANAGERS 1
//#define AUCTION_ROOM 2
//#define MANAGE_PLAYERS 3
//#define MANAGE_BUILDINGS 4
        case SEE_MANAGERS : {
            state_ = MANAGERS_MENU;
            break;
        }
        case AUCTION_ROOM : {
            state_ = AUCTION_MENU;
            break;
        }
        case MANAGE_PLAYERS : {
            state_ = PLAYERS_MENU;
            break;
        }
        case MANAGE_BUILDINGS : {
            state_ = BUILDINGS_MENU;
            break;
        }
        case ACTION_POINTS : {
            state_ = AP_MENU;
            break;
        }
        case 0 : {
			state_ = DISCONNECTING;
			break;
		}
        

    }
}

void Client::handleOpponentChoice(){
    if(input_ == -1){
        state_ = FREE;
        return;
    }
    //TODO : tester input
    //verrou du contexte après le choix d'un adversaire
    std::vector<int> playersInTeam = displayAndAskPlayersForMatch();
    proposeMatchTo(input_,  playersInTeam);
    if(receiveMatchConfirmation() == MATCH_STARTING){
        startMatch(1); //inviteur a l'équipe 1
        state_ = FREE; //on suppose que la fonction ne retourne qu'à la fin du match
    }
}

void Client::startMatch(int numTeam){
    int winner = 0;
    int scoreTeam1 = 0;
    int scoreTeam2 = 0;
    HexagonalField field;
    std::vector<AxialCoordinates> allPositions;
    getAllPositions();
    allPositions = receiveScoresAndPositions(&winner, &scoreTeam1, &scoreTeam2);
    while(winner == 0){
        winner = testifContinue(numTeam); //demande match nul ou forfait + écoute socket pour match nul ou socket
        if(winner == 0){ 
            cout << "Your are team n° " << numTeam << endl;
            cout << "Score team1 (inviter) = " << scoreTeam1 << endl;
            cout << "Score team2 (invited) = " << scoreTeam2 << endl;
            field.reset();
            //TODO : méthode de field :
            for(unsigned int i = 0; i < allPositions.size(); ++i){
              if(field.getOccupant(allPositions[i]) == FREE_SPACE){
                field.setOccupant(allPositions[i], i);
              }
            }
            field.display();
            //TODO : tester si balle superposée à un joueur, et indiquer le joueur le cas échéant
            askAndSendMoves(numTeam, field, allPositions);
            getConfirmation();
            
            //cout << "Les échanges de messages suivants pour le match n'ont pas encore été implémentés." << endl;
            //winner =1;
            getAllPositions();
            allPositions = receiveScoresAndPositions(&winner, &scoreTeam1, &scoreTeam2);
        }
    }
    //TODO : gérer demande de match nul
    cout << "Winner is team " << winner << " (-1 is a draw)"<< endl;
}

int Client::testifContinue(int numTeam){
    int winner = 0;
    cout << "You are in a match, what do you want to do :  " << numTeam << endl;
    cout << "[1] Continue the tour " << endl;
    cout << "[2] Forfeit "  << endl;
    cout << "[3] Suggest a draw (in a tournament, the one who suggest is considered as the loser) " << endl;
    loadFDSet();
    if (select(sockfd_+1,&FDSet_,NULL,NULL,NULL)==ERROR) {
            std::cerr<<"Socket bind error"<<std::endl;
            return EXIT_FAILURE;
    }
    if(keyboard()){ // si entrée clavier
        cin>>input_; //on récupère l'entrée
        switch(input_){
            case 1 : {
                return winner;
            }
            case 2 : {
                sendForfeit(); //pas besoin d'une confirmation
                return numTeam == 1 ? 2 : 1; //team adverse gagne
            }
            case 3 : {
                sendDrawRequest();
                int result = getConfirmation();
                if(result == DRAWACCEPTED){
                    cout << "Draw accepted by the opponent ! " << endl;
                    return -1;
                }else{
                    return winner;
                }
            }
        }
    }
    else{ //sinon, message du serveur
        SerializedObject received = receiveOnSocket(sockfd_);
        switch(received.typeOfInfos){
            case OPPONENTFORFEIT : {
                cout << "Opponent forfeit !" << endl;
                return numTeam;
            }
            case OPPONENTASKFORDRAW : {
                int code;
                cout << "Opponent ask for a draw ! (in a tournament, the one who suggest is considered as the loser)" << endl;
                cout << "Do you accept ? [1 for yes/0 for no] " << endl;
                cin >> input_;
                if(input_){
                    code = DRAWACCEPTED;
                }else{
                    code = DRAWDENIED;
                }
                sendAnswerToDrawProposition(code);
                return input_ ? -1 : 0;
            }
        }
    }
    
}

int Client::sendAnswerToDrawProposition(int code){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = ANSWERTODRAWPROPOSITION;
    memcpy(position, &code, sizeof(code));
    return sendOnSocket(sockfd_, serialized);
}

int Client::sendForfeit(){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = FORFEIT;
    return sendOnSocket(sockfd_, serialized);
}

int Client::sendDrawRequest(){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = ASKFORDRAW;
    return sendOnSocket(sockfd_, serialized);
}

void Client::askAndSendMoves(int numTeam, HexagonalField &field, std::vector<AxialCoordinates> &positions){
	//TODO : inutile de récupérer la position du joueur lors de la sélection, déjà contenue dans le vecteur positions
        //TODO : spliter le code
  int moves[7][4];
  int playerRole = 0;
  int currentMove = 0;
  int selectedPlayerID = 1;
  int choiceInput, deltaDiag, deltaLine, distanceAccepted;
  playerAttr attributs;
  std::string keeper, seeker, chaser1, chaser2, chaser3, beater1, beater2, quaffle, bludger, goldensnitch;
  int nearestBludger = BLUDGER1;
  if(numTeam == 1){ //TODO : optimiser
    keeper = TEAM1_KEEPER_UNICODE;
    seeker = TEAM1_SEEKER_UNICODE;
    chaser1 = TEAM1_CHASER1_UNICODE;
    chaser2 = TEAM1_CHASER2_UNICODE;
    chaser3 = TEAM1_CHASER3_UNICODE;
    beater1 = TEAM1_BEATER1_UNICODE;
    beater2 = TEAM1_BEATER2_UNICODE;
  }
  else{
    keeper = TEAM2_KEEPER_UNICODE;
    seeker = TEAM2_SEEKER_UNICODE;
    chaser1 = TEAM2_CHASER1_UNICODE;
    chaser2 = TEAM2_CHASER2_UNICODE;
    chaser3 = TEAM2_CHASER3_UNICODE;
    beater1 = TEAM2_BEATER1_UNICODE;
    beater2 = TEAM2_BEATER2_UNICODE;
  }
  quaffle = QUAFFLE_UNICODE;
  bludger = BLUDGER1_UNICODE;
  goldensnitch = GOLDENSNITCH_UNICODE;
  
  //initialisation de la matrice des mouvements : liste de mouvements vides
  //TODO : à n'initialiser qu'une fois, à la création de l'instance client,à l'instar de User
  for(int i = 0; i < 7; ++i){
    moves[i][0] = i;
    moves[i][1] = NO_SPECIAL_ACTION;
    moves[i][2] = 10000; //sentinelle : mouvement vide
    moves[i][3] = 10000;
  }
  
  while((playerRole != -1) and (currentMove < 7)){
    std::cout << "Légende :" << endl;
    std::cout << "KEEPER : " << keeper << std::endl;
    std::cout << "SEEKER  : " << seeker << std::endl;
    std::cout << "CHASER1  : " << chaser1 << std::endl;
    std::cout << "CHASER2  : " << chaser2 << std::endl;
    std::cout << "CHASER3  : " << chaser3 << std::endl;
    std::cout << "BEATER1  : " << beater1 << std::endl;
    std::cout << "BEATER2 : " << beater2 << std::endl;
    std::cout << "QUAFFLE : " << quaffle << std::endl;
    std::cout << "BLUDGER : " << bludger << std::endl;
    std::cout << "GOLDENSNITCH : " << goldensnitch << std::endl;
    std::cout << "Veuillez sélectionner un joueur(valeur numérique), -1 pour terminer le tour :" << endl;
    //TODO : vérification de l'input : indice correspond bien à un joueur de l'équipe et pas déjà de mouvement attribué
    std::cin >> playerRole;
    if(playerRole != -1){
      playerRole -= 1; //index commence à 0
      selectedPlayerID = playerRole;
      if(numTeam == 2){
        selectedPlayerID += 7; //ajustement de l'index en fonction de l'équipe
      }
      selectPlayer(selectedPlayerID);
      attributs = receiveSelectedPlayerInfos();
      field.display(attributs.position, attributs.attributes[SPEED]);
      std::cout << "Attributs du joueur sélectionné  : " << std::endl;
      std::cout << "Vitesse  : " << attributs.attributes[SPEED] << std::endl;
      std::cout << "Force  : " << attributs.attributes[STRENGTH] << std::endl;
      std::cout << "Précision  : " << attributs.attributes[PRECISION] << std::endl;
      std::cout << "Réflexe  : " << attributs.attributes[REFLEX] << std::endl;
      std::cout << "Résistance  : " << attributs.attributes[RESISTANCE] << std::endl;
      if(attributs.hasQuaffle){
        std::cout << "Ce joueur porte le souaffle " << std::endl;
      }
      else{
        std::cout << "Ce joueur ne porte pas le souaffle " << std::endl;
      }
      //RAPPEL : 
      //moves[][0] : indice de l'objet concerné
      //moves[][1] : action spéciale :
      //#define INTERCEPT_QUAFFLE 0
      //#define CATCH_GOLDENSNITCH 1
      //moves[][2] : diagonale destination
      //moves[][3] : ligne destination
      std::cout << "Vos possibilités pour ce joueur :" << std::endl;
      std::cout << "[0]Déplacer le joueur" << std::endl;
      if(attributs.hasQuaffle){
        std::cout << "[1]Envoyer le souaffle dans une direction" << std::endl;
      }
      else if((playerRole <= TEAM1_BEATER2) and (playerRole >= TEAM1_BEATER1)){
        
        if (attributs.position.getDistanceTo(positions[BLUDGER2]) < attributs.position.getDistanceTo(positions[BLUDGER1])){
          nearestBludger = BLUDGER2;        }
        if(attributs.position.getDistanceTo(positions[nearestBludger]) < 2 ){ //seulement si adjacent
          std::cout << "[1]Frapper le cognard dans une direction" << std::endl;
        }
      }
      //menus proposés seulement si précision suffisante par rapport à la distance avec la balle
      //TODO : que faire si souaffle à distance accessible mais déjà possédé par un chaser ?
      else if((((playerRole <= TEAM1_CHASER3) and (playerRole>= TEAM1_CHASER1)) or (playerRole == TEAM1_KEEPER))
      and (attributs.position.getDistanceTo(positions[QUAFFLE]) <= attributs.attributes[PRECISION])){
        std::cout << "[2]Tenter de récupérer le souaffle" << std::endl;
      }
      else if((playerRole == TEAM1_SEEKER)
      and (attributs.position.getDistanceTo(positions[GOLDENSNITCH]) <= attributs.attributes[PRECISION]) ){
        std::cout << "[2]Tenter d'attraper le vif d'or" << std::endl;
      }
      
      std::cin >> choiceInput;
      if(choiceInput == 0){
        std::cout << "Déplacement le long de la ligne -- (négatif vers la gauche, positif vers la droite) : " << std::endl;
        std::cin >> deltaDiag;
        std::cout << "Déplacement le long de la diagonale \\ (négatif pour monter, positif pour descendre) : " << std::endl;
        std::cin >> deltaLine;
        if(attributs.position.getDistanceTo(AxialCoordinates(attributs.position.getDiagAxis() + deltaDiag, attributs.position.getLineAxis() + deltaLine)) <= attributs.attributes[SPEED]){
          //si case accessible
          moves[currentMove][0] = selectedPlayerID;
          moves[currentMove][1] = NO_SPECIAL_ACTION; //pas d'action spéciale
          moves[currentMove][2] = attributs.position.getDiagAxis() + deltaDiag;
          moves[currentMove][3] = attributs.position.getLineAxis() + deltaLine;
        }else{
          std::cout << "Case trop éloignée, votre joueur n'a pas une vitesse suffisante !" << std::endl;
        }
      }
      //interception d'une balle
      else if(choiceInput == 2){ 
        if(playerRole == TEAM1_SEEKER){
          moves[currentMove][0] = selectedPlayerID;
          moves[currentMove][1] = CATCH_GOLDENSNITCH;
        }
        else if(((playerRole <= TEAM1_CHASER3) and (playerRole>= TEAM1_CHASER1)) or (playerRole == TEAM1_KEEPER)){
          moves[currentMove][0] = selectedPlayerID;
          moves[currentMove][1] = INTERCEPT_QUAFFLE;
        }
      }
      //frappage/lancement d'une balle : déplacement du joueur remplacé par déplacement de la balle
      else if(choiceInput == 1){ 
        if((playerRole == TEAM1_BEATER1) or (playerRole == TEAM1_BEATER2)){
          distanceAccepted = attributs.attributes[STRENGTH];
          field.display(positions[nearestBludger], distanceAccepted); 
          //pour le moment, la taille du déplacement du cognard quand frappé ne dépend que de l'attribut force
          std::cout << "Déplacement du cognard le long de la ligne -- (négatif vers la gauche, positif vers la droite) : " << std::endl;
          std::cin >> deltaDiag;
          std::cout << "Déplacement du cognard le long de la diagonale \\ (négatif pour monter, positif pour descendre) : " << std::endl;
          std::cin >> deltaLine;
          if(positions[nearestBludger].getDistanceTo(AxialCoordinates(positions[nearestBludger].getDiagAxis() + deltaDiag, positions[nearestBludger].getLineAxis() + deltaLine)) <= distanceAccepted){
          //si case accessible
            moves[currentMove][0] = nearestBludger;
            moves[currentMove][1] = NO_SPECIAL_ACTION;
            moves[currentMove][2] = positions[nearestBludger].getDiagAxis() + deltaDiag;
            moves[currentMove][3] = positions[nearestBludger].getLineAxis() + deltaLine;
          }else{
            std::cout << "Hors du terrain ou votre joueur n'est pas assez fort pour l'envoyer si loin !" << std::endl;
          }
        }
        else if(attributs.hasQuaffle){
          distanceAccepted = attributs.attributes[STRENGTH];
          field.display(positions[QUAFFLE], distanceAccepted); 
          //TODO : pour marquer un but il faut que la trajectoire du souaffle soit une ligne
          std::cout << "Déplacement du souaffle le long de la ligne -- (négatif vers la gauche, positif vers la droite) : " << std::endl;
          std::cin >> deltaDiag;
          std::cout << "Déplacement du souaffle le long de la diagonale \\ (négatif pour monter, positif pour descendre) : " << std::endl;
          std::cin >> deltaLine;
          if(positions[selectedPlayerID].getDistanceTo(AxialCoordinates(positions[selectedPlayerID].getDiagAxis() + deltaDiag, positions[selectedPlayerID].getLineAxis() + deltaLine)) <= distanceAccepted){
            //si case accessible
            moves[currentMove][0] = QUAFFLE;
            moves[currentMove][1] = NO_SPECIAL_ACTION;
            moves[currentMove][2] = attributs.position.getDiagAxis() + deltaDiag;
            moves[currentMove][3] = attributs.position.getLineAxis() + deltaLine;
          }else{
            std::cout << "Hors du terrain ou votre joueur n'est pas assez fort pour l'envoyer si loin !" << std::endl;
          }
        }
      } 
          
      //si input correct, on passe au joueur suivant
      ++currentMove;
    }
  }
  //TODO : tester si reception d'une demande de match nul ou forfait
  sendMoves(moves);
}



void Client::commMgr() {
//gère les messages non sollicités (exemple : invitation à un match amical)
	SerializedObject received = receiveOnSocket(sockfd_);
	switch(received.typeOfInfos){
		case MATCH_INVITATION : {
            int IDInvitor;
            char name[USERNAME_LENGTH];
            char * position = received.stringData;
            cout << "You've got a match proposal !" << endl;
            memcpy(&IDInvitor, position, sizeof(IDInvitor));
            position += sizeof(IDInvitor);
            memcpy(&name, position, sizeof(name));
            cout << "Opponent ID : " << IDInvitor << " name : " << name << endl;
            cout << "Accept ? [1/0]" << endl;
            bool confirmation;
            cin >> confirmation;
            std::vector<int> playersInTeam;
            if(confirmation){
                playersInTeam = displayAndAskPlayersForMatch();
            }
            answerMatchProposal(confirmation, playersInTeam); //liste vide = refus de l'invitation
            if(receiveMatchConfirmation() == MATCH_STARTING){
                startMatch( 2); //invité a l'équipe 2
                state_ = FREE;
            }
        }
	}
}

void Client::matchTentative() {
//    msg[0]='M';
//    msg[1]=' ';
//        std::cout<<"Managers disponibles :"<<std::endl;
//        contactServer();
//        if(msg[1]==' ') {//il y a des managers disponibles
//                state_=MATCH_INVITING;
//                std::cin>>input_; //nom du manager choisi comme adversaire
//                msg[1]='S';
//                strcpy(opponent_,input_);
//                strcpy(&msg[2],opponent_);
//                contactServer();
//                if(msg[1]==' ') //le match peut commencer
//                        state_=MATCH_INGAME;
//                        std::cout<<"C'est parti pour un match amical !"<<std::endl;
//// le nom de mon adversaire se trouve dans opponent_
////                      startGame()
//                state_=FREE;
//        }
}

void Client::contactServer() {
        //sendMsg(sockfd_,msg,strlen(msg));
        //receiveMessage();
}
void Client::receiveMessage() {
        //receiveMsg(sockfd_,msg,INPUTSIZE);
        //std::cout<<&msg[2]<<std::endl;
}

////////Network\\\\\\\\

//login\\

int Client::sendLoginToServer(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]){
    //REM : les param reçu ne sont pas des char[N] (sizeof() = N) mais des char * (sizeof() = taille pointeur)
    SerializedObject serialized;
    char * position;
    serialized.typeOfInfos = LOGIN;
    position = serialized.stringData;
    memcpy(position, username, USERNAME_LENGTH);
    position += USERNAME_LENGTH;
    memcpy(position, password, PASSWORD_LENGTH);
    //position += sizeof(password);
    return sendOnSocket(sockfd_, serialized);
}

int Client::sendNewManagerToServer(char username[USERNAME_LENGTH], char password[PASSWORD_LENGTH]){
    //REM : les param reçu ne sont pas des char[N] (sizeof() = N) mais des char * (sizeof() = taille pointeur)
    //TODO : redondance avec sendLogin
    SerializedObject serialized;
    char * position;
    serialized.typeOfInfos = CREATE_MANAGER;
    position = serialized.stringData;
    memcpy(position, username, USERNAME_LENGTH);
    position += USERNAME_LENGTH;
    memcpy(position, password, PASSWORD_LENGTH);
    //position += sizeof(password);
    return sendOnSocket(sockfd_, serialized);
}

int Client::getConfirmation(){ //valable pour LOGIN_CONFIRM, UPGRADE_CONFIRM, TRAINING_STARTED, HEAL_STARTED, AUCTIONCREATION_CONFIRM
    int confirmation;
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    //TODO : vérifier qu'il s'agit bien d'un message de confirmation
    memcpy(&confirmation,position, sizeof(confirmation));
    return confirmation;
}

//management\\

int Client::askForManagerInfos(){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = GETMANAGERINFOS;
    return sendOnSocket(sockfd_, serialized);
}

void Client::receiveManagerInfos(int *nbPlayers, int * money, int * nbFans, int * actionPoints){
    //écrit les données sur les objets pointés
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    if(received.typeOfInfos == MANAGERINFOS){
        memcpy(nbPlayers,position, sizeof(int));
        position += sizeof(int);
        memcpy(money,position, sizeof(int));
        position += sizeof(int);
        memcpy(nbFans,position, sizeof(int));
        position += sizeof(int);
        memcpy(actionPoints,position, sizeof(int));
        position += sizeof(int);
    }
}

int Client::askForBuildingInfos(int buildingID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = GETBUILDINGINFOS;
    memcpy(position, &buildingID, sizeof(buildingID));
    return sendOnSocket(sockfd_, serialized);
}

std::vector<int> Client::receiveBuildingInfos(){
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    std::vector<int> buildingInfos;
    if(received.typeOfInfos == BUILDINGINFOS){ //on suppose toujours vrai
            //int : level
            //int : pricefornextlevel
            //int : specialAttribut
            //int : enTravaux
        for(int i = 0; i < 4; ++i){
            int value;
            memcpy(&value,position, sizeof(value));
            position += sizeof(value);
            buildingInfos.push_back(value); //ajout à la liste
        }
    }
    return buildingInfos;
}

int Client::askForBuildingUpgrade(int buildingID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = UPGRADE_BUILDING;
    memcpy(position, &buildingID, sizeof(buildingID));
    return sendOnSocket(sockfd_, serialized);
}

int Client::askForPlayersList(){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = GETPLAYERSLIST;
    return sendOnSocket(sockfd_, serialized);
}

std::vector<std::string> Client::receivePlayersList(){
    //pour chaque ManagedPlayer, on reçoit un string firstname et un lastname, de façon ordonnée (indice 0 à 1 : playerID = 0, etc)
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    std::vector<std::string> playersListNames;
    if(received.typeOfInfos == PLAYERSLIST){ //on suppose toujours vrai
        int nbNames;
        memcpy(&nbNames,position, sizeof(nbNames));
        position += sizeof(nbNames);
        std::cout << nbNames << std::endl;
        for(int i = 0; i < nbNames; ++i){
            char name[USERNAME_LENGTH];
            std::string strName;
            memcpy(&name,position, sizeof(name));
            position += sizeof(name);
            strName = name; //conversion
            playersListNames.push_back(strName); //ajout à la liste
        }
    }
    return playersListNames;
}

int Client::askForPlayerInfos(int playerID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = GETPLAYERINFOS;
    memcpy(position, &playerID, sizeof(playerID));
    return sendOnSocket(sockfd_, serialized);
}

std::vector<int> Client::receivePlayerInfo(){
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    std::vector<int> playerInfos;
    if(received.typeOfInfos == PLAYERINFOS){ //on suppose toujours vrai
            //5 attributs int
			//5 états d'entrainements d'attribut int
			//1 int blocked
			//1 int bonus du balais
			//1 int capacity du balais
            //1 int life
            //1 int estimated value of player
        for(int i = 0; i < 15; ++i){
            int value;
            memcpy(&value,position, sizeof(value));
            position += sizeof(value);
            playerInfos.push_back(value); //ajout à la liste
        }
    }
    return playerInfos;
}

int Client::trainPlayer(int playerID, int capacity){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = TRAIN_PLAYER;
    memcpy(position, &playerID, sizeof(playerID));
    position += sizeof(playerID);
    memcpy(position, &capacity, sizeof(capacity));
    return sendOnSocket(sockfd_, serialized);
}

int Client::healPlayer(int playerID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = HEAL_PLAYER;
    memcpy(position, &playerID, sizeof(playerID));
    return sendOnSocket(sockfd_, serialized);
}

//match\\

int Client::getManagersList(){
    SerializedObject serialized;
    serialized.typeOfInfos = GETMANAGERSLIST;
    return sendOnSocket(sockfd_, serialized);
}

void Client::receiveManagersIDandNames(std::vector<int> * IDList, std::vector<std::string> * namesList){
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    int ID;
    char name[USERNAME_LENGTH];
    int nbOfAvailableManagers;
    memcpy(&nbOfAvailableManagers, position, sizeof(nbOfAvailableManagers));
    position += sizeof(nbOfAvailableManagers);
    for(int i = 0; i < nbOfAvailableManagers; ++i){
        memcpy(&ID, position, sizeof(ID));
        position += sizeof(ID);
        IDList->push_back(ID);
        memcpy(&name, position, sizeof(name));
        position += sizeof(name);
        std::string strName = name;
        namesList->push_back(strName);
    }
}

int Client::proposeMatchTo(int userID, std::vector<int> playersInTeam){
    SerializedObject serialized;
    char * position = serialized.stringData;
    int value;
    serialized.typeOfInfos = PROPOSEMATCH;
    memcpy(position, &userID, sizeof(userID));
    position += sizeof(userID);
    for(unsigned int i = 0; i < playersInTeam.size();++i){ //joueurs choisi pour joueur le match
        value = playersInTeam[i];
        memcpy(position, &value, sizeof(value));
        position += sizeof(value);
    }
    
    return sendOnSocket(sockfd_, serialized);
}

int Client::answerMatchProposal(bool confirmation, std::vector<int> playersInTeam){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = ACCEPTMATCH;
    memcpy(position, &confirmation, sizeof(confirmation));
    position += sizeof(confirmation);
    int value;
    for(unsigned int i = 0; i < playersInTeam.size();++i){ //joueurs choisi pour joueur le match
        value = playersInTeam[i];
        memcpy(position, &value, sizeof(value));
        position += sizeof(value);
    }
    return sendOnSocket(sockfd_, serialized);
}

int Client::receiveMatchConfirmation(){
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    int confirmation;
    memcpy(&confirmation, position, sizeof(confirmation));
    return confirmation;
}

int Client::getAllPositions(){
    SerializedObject serialized;
    serialized.typeOfInfos = GETPOSITIONS;
    return sendOnSocket(sockfd_, serialized);
}

std::vector<AxialCoordinates> Client::receiveScoresAndPositions(int * winner, int * scoreTeam1, int * scoreTeam2){
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    std::vector<AxialCoordinates> orderedPositions;
    int diag;
    int line;
    memcpy(winner, position, sizeof(int));
    position += sizeof(int);
    memcpy(scoreTeam1, position, sizeof(int));
    position += sizeof(int);
    memcpy(scoreTeam2, position, sizeof(int));
    position += sizeof(int);
    for(unsigned int i = 0; i < 18; ++i){ //positions des 18 objets
        memcpy(&diag, position, sizeof(diag));
        position += sizeof(diag);
        memcpy(&line, position, sizeof(line));
        position += sizeof(line);
        orderedPositions.push_back(AxialCoordinates(diag, line));
    }
    return orderedPositions;
}

int Client::selectPlayer(int playerID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = SELECTPLAYER;
    memcpy(position, &playerID, sizeof(playerID));
    return sendOnSocket(sockfd_, serialized);
}

playerAttr Client::receiveSelectedPlayerInfos(){
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    playerAttr thePlayer;
    int attribute, diag, line;
    for(int i = 0; i < 5; ++i){
        memcpy(&attribute, position, sizeof(attribute));
        position += sizeof(attribute);
        thePlayer.attributes[i] = attribute;
    }
    memcpy(&diag, position, sizeof(diag)); //coord 1 : diagonale
    position += sizeof(diag);
    memcpy(&line, position, sizeof(line)); //coord 2 : ligne
    position += sizeof(line);
    thePlayer.position = AxialCoordinates(diag, line);
    memcpy(&attribute, position, sizeof(attribute)); //hasQuaffle
    position += sizeof(attribute);
    thePlayer.hasQuaffle = attribute;
    return thePlayer;
}

int Client::sendMoves(int moves[][4]){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = MAKEMOVES;
    int targetedPlayer;
    int specialAction;
    int diagDest;
    int lineDest;
    for(int i = 0; i < 7; ++i){
        targetedPlayer = moves[i][0];
        specialAction = moves[i][1];
        diagDest = moves[i][2];
        lineDest = moves[i][3];
        printf("%s : %d %d %d\n", "playerID diagDest lineDest", targetedPlayer, diagDest, lineDest);
        memcpy(position, &targetedPlayer,sizeof(targetedPlayer)); 
        position += sizeof(targetedPlayer);
        memcpy(position, &specialAction,sizeof(specialAction)); 
        position += sizeof(specialAction);
        memcpy(position, &diagDest, sizeof(diagDest));
        position += sizeof(diagDest); 
        memcpy(position, &lineDest, sizeof(lineDest));
        position += sizeof(lineDest); 
    }
    return sendOnSocket(sockfd_, serialized);
}



//TODO : useless
//int Client::isMatchWaiting(){
//    SerializedObject serialized;
//    char * position = serialized.stringData;
//    serialized.typeOfInfos = IS_MATCH_WAITING;
//    return sendOnSocket(sockfd_, serialized);
//}

//auctions\\

int Client::sellPlayer(int playerID, int startingPrice){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = CREATEAUCTION;
    memcpy(position, &playerID, sizeof(playerID));
    position += sizeof(playerID);
    memcpy(position, &startingPrice, sizeof(startingPrice));
    return sendOnSocket(sockfd_, serialized);
}

int Client::getAuctionsList(){
    SerializedObject serialized;
    serialized.typeOfInfos = GETAUCTIONSLIST;
    return sendOnSocket(sockfd_, serialized);
}

std::vector<std::string> Client::receiveAuctionsList(){
    //pour chaque ManagedPlayer, on reçoit un string firstname et un lastname, de façon ordonnée (indice 0 à 1 : playerID = 0, etc)
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    std::vector<std::string> auctionsList;
    if(received.typeOfInfos == AUCTIONSLIST){ //on suppose toujours vrai
        int nbInfos;
        memcpy(&nbInfos,position, sizeof(nbInfos));
        position += sizeof(nbInfos);
        std::cout << nbInfos << std::endl;
        for(int i = 0; i < nbInfos; ++i){
            char info[2*USERNAME_LENGTH];
            std::string strInfos;
            memcpy(&info,position, sizeof(info));
            position += sizeof(info);
            strInfos = info; //conversion
            auctionsList.push_back(strInfos); //ajout à la liste
        }
    }
    return auctionsList;
}

int Client::joinAuction(int auctionID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = JOINAUCTION;
    memcpy(position, &auctionID, sizeof(auctionID));
    return sendOnSocket(sockfd_, serialized);
}

int Client::askForAuctionInfos(int auctionID){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = GETAUCTIONINFO;
    memcpy(position, &auctionID, sizeof(auctionID));
    return sendOnSocket(sockfd_, serialized);
}

int Client::getCurrentPrice(){
    SerializedObject serialized;
    serialized.typeOfInfos = GETCURRENTPRICE;
    return sendOnSocket(sockfd_, serialized);
}

int Client::bid(){
    SerializedObject serialized;
    serialized.typeOfInfos = BID;
    return sendOnSocket(sockfd_, serialized);
}

int Client::checkAuction(){
    SerializedObject serialized;
    serialized.typeOfInfos = END_AUCTION_TURN;
    return sendOnSocket(sockfd_, serialized);
}

int Client::receiveAuctionResult() {
    int result;
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    //TODO : vérifier qu'il s'agit bien d'un message de confirmation
    memcpy(&result,position, sizeof(result));
    return result;
}

int Client::startPromotionCampaign(){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = START_PROMOTION;
    return sendOnSocket(sockfd_, serialized);
}

int Client::endPromotionCampaign(){
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = END_PROMOTION;
    return sendOnSocket(sockfd_, serialized);
}

int Client::getPromotionResult() {
    int result;
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    //TODO : vérifier qu'il s'agit bien d'un message de confirmation
    memcpy(&result,position, sizeof(result));
    return result;
}

int Client::buyActionPoints(int amount) {
    SerializedObject serialized;
    char * position = serialized.stringData;
    serialized.typeOfInfos = BUY_ACTION_POINTS;
    memcpy(position, &amount, sizeof(amount));
    return sendOnSocket(sockfd_, serialized);
}

int Client::getPriceForAP(){    
    int result;
    SerializedObject received = receiveOnSocket(sockfd_);
    char * position = received.stringData;
    //TODO : vérifier qu'il s'agit bien d'un message de confirmation
    memcpy(&result,position, sizeof(result));
    return result;
}

void *auctionTurn(void* data) {

  //int* sockfd = (int *)data;
  Client *client = (Client *) data;
    //TODO : si solution conservée, mutexes
  cout<<"Auction turn has started !"<<endl;
  cout<<" [1] Bid "<<endl;
  cout<<" [2] Quit auction"<<endl;


  int choice;
  cout<<"-----> ";
  cin>>choice;

  if (choice==1) {
    client->bid();
    int result = client->getConfirmation();
    cout << "Bid done, waiting for the end of the turn. " << endl;
    isBidder=true;
    hasChosen=true;
  }
  else {
    isBidder=false;
    hasChosen=true;
  }

}

void Client::mainAuction(int auctionID, int timeLeft) {
    //TODO : deux appels à receiveAuctionResult : bizarre
  hasChosen=false;
  isBidder=false;
  int turn = 0;

  pthread_t thread;
  int res = pthread_create(&thread, NULL, auctionTurn, (void*) this);
  sleep(timeLeft);
  pthread_cancel(thread);
  checkAuction();
  int result = receiveAuctionResult();
  int price = receiveAuctionResult();
  cout << "result : " << result << " price " << price << endl;
  if (result<0) {
    cout<<" -Congrats! You have won this auction"<<endl;
    if (result==-1) cout<<" --- "<<price<<" gold will be transferred in exchange of the player"<<endl;
    else if (result==-2) {
      cout<<" --- You don't have enough money to pay the "<<price<<" required";
      cout<<", so 3 percent of your money will be given to the owner of the player"<<endl;
    }
  }
  else if (result==0) {
    cout<<" -Goodbye! You have left this auction"<<endl;
  }
  else {
    cout<<" -Next turn will start soon... The current price is "<<price<<endl;
    sleep(10);
    turn+=1;

    while ((result>0)&&(hasChosen)&&(isBidder)) {
        hasChosen=false;
        res = pthread_create(&thread, NULL, auctionTurn, (void*) this);
        sleep(30);
        pthread_cancel(thread);
        cout<<" --- END OF TURN"<<endl;
        checkAuction();
        result = receiveAuctionResult();
        price = receiveAuctionResult();
        if ((!hasChosen)||(!isBidder)) {
          cout<<" -Goodbye! You have left this auction"<<endl;
        }
        else if (result<0) {
          cout<<" -Congrats! You have won this auction"<<endl;
          if (result==-1) cout<<" --- "<<price<<" gold will be transfered in exchange of the player"<<endl;
          else if (result==-2) {
            cout<<" --- You don't have enough money to pay the "<<price<<" required";
            cout<<", so 3 percent of your money will be given to the owner of the player"<<endl;
          }
        }
        else {
          cout<<" -Next turn will start soon... The current price is "<<price<<endl;
          sleep(10);
        }
        turn+=1;
      } 
    }
}



void Client::handleAuctions(){
    if (input_==SEE_AUCTIONS) {
        getAuctionsList();
        vector<string> auctionsList = receiveAuctionsList();
        displayAuctionsList(auctionsList);
        if (auctionsList.size()!=0) {
            int auctionToInspect;
            cout<<" -----> ";
            cin>>auctionToInspect;
            if (auctionToInspect!=0) {
                askForAuctionInfos(auctionToInspect-1);
                vector<int> playerAuctionInfos = receivePlayerInfo();
                string timeLeft = displayAuctionInfos(auctionsList,playerAuctionInfos,auctionToInspect-1);
                cout<<" ---- Entering an auction costs "<<AP_ENTER_AUCTION<<" action points ----"<<endl;
                cout<<"Do you want to join this auction ? [1 to enter, 0 to quit] \n -----> ";
                int enterAuction;
                cout<<" -----> ";
                cin>>enterAuction;
                if (enterAuction==1) {
                    joinAuction(auctionToInspect-1);
                    int joinResult = getConfirmation();
                    mainAuction(auctionToInspect-1,atoi(timeLeft.c_str()));
                }
            }
        }
    }
    else if (input_==SELL_PLAYER) {
        int sellPlayerChoice;
        do {
            displayManagerInfos();
            displaySellPlayerMenu();
            cin>>sellPlayerChoice;
            if (sellPlayerChoice!=ABORT) {
                askForPlayerInfos(sellPlayerChoice-1);
                vector<int> playerInfos = receivePlayerInfo();
                displayPlayerInfos(playerInfos,sellPlayerChoice-1);
                cout<<"Indicate the starting price of the auction [or 0 to abort] \n -----> ";
                int startingPrice;
                cin>>startingPrice;
                if (startingPrice>0) {
                    sellPlayer(sellPlayerChoice-1,startingPrice);
                    bool sellingResult = getConfirmation(); //always true
                    if (!sellingResult) cout<<" ----- The player is blocked and cannot be selled right now"<<endl;
                    else cout<<" ----- Auction started !"<<endl;
                    sellPlayerChoice=ABORT;
                }
            }
        } while (sellPlayerChoice!=ABORT);
    }
    else if (input_==ABORT) state_ = FREE;
}

void Client::handlePlayersMenu(){
    switch(input_){
        case ABORT : {
            state_ = FREE;
            return;
        }
        case INSPECT_PLAYER : {
            state_ = PLAYERSLIST_MENU;
            break;
        }
        case TRAIN_PLAYER_OPTION : {
            state_ = TRAINING_MENU;
            break;
        }
        case HEAL_PLAYER_OPTION : {
            state_ = HEALING_MENU;
            break;
        }
    }
}

void Client::kbMgr() {
    cin>>input_;
    //TODO : s'assurer de la gestion des retours en arrière
    //TODO : tester input
    switch(state_) {
        case INIT : {
            //contexte verrouillé, pas d'écoute du socket tant que login pas terminé
            handleLogin();
            break;
        }
        case FREE : {
            
        //affichage du sous-menu avec adaptation du contexte
            handleMainMenu();
            
            break;
        }
        case MANAGERS_MENU : {
            //verrouillage du contexte
            handleOpponentChoice();
            break;
        }
        
        case AUCTION_MENU : {
            //verrouillage du contexte
            handleAuctions();
            break;
        }
        case PLAYERS_MENU : {
            handlePlayersMenu();
            break;
        }
        
        case PLAYERSLIST_MENU : {
            if (input_>0) {
                askForPlayerInfos(input_-1);
                vector<int> playerInfos = receivePlayerInfo();
                displayPlayerInfos(playerInfos,input_-1);
            }
            state_ = FREE;
            break;
        }
        case TRAINING_MENU : {
            //contexte verrouillé par facilité
            if(input_ != ABORT){
				int capacityNumber;
				cout<<"Indicate the number of the capacity you wish to train [or 0 to abort]  \n-----> ";
				cin>>capacityNumber;
				if ((input_>0)&&(capacityNumber>0)) {
				  trainPlayer(input_-1,capacityNumber-1);
				  bool trainingResult = getConfirmation();
				  if (trainingResult) {
					askForBuildingInfos(TRAININGCENTER);
					vector<int> trainingCenterInfos = receiveBuildingInfos();
					cout<<"-------------\n Training has started !"<<endl;
					cout<<" Training will be over in "<<trainingCenterInfos[2]<<" minutes"<<endl;
				  }
				  else cout<<"------------\n Training impossible, this player is blocked"<<endl;
				}
			}
            state_ = FREE;
            break;
        }
        case HEALING_MENU : {
            if (input_!=ABORT) {
                healPlayer(input_-1);
                int healResult = getConfirmation();
                if (healResult){
                    askForBuildingInfos(HOSPITAL);
                    vector<int> hospitalInfos = receiveBuildingInfos();
                    cout<<"-------------\n This player has been sent to the hospital !"<<endl;
                    cout<<" Player will leave the hospital in "<<hospitalInfos[2]<<" minutes"<<endl;
                }
                else cout<<"-------------\n This player cannot be send to the hospital, he's blocked or in full health or you don't have enough action points"<<endl;
            }
            state_ = FREE;
            break;
        }
        
        case BUILDINGS_MENU : {
			//contexte verrouillé par facilité
			if(input_ != 0){
				askForBuildingInfos(input_);
				vector<int> buildingInfos = receiveBuildingInfos();
				displayBuildingInfos(buildingInfos,input_);
                cout<<"\n ---- Upgrading a building costs "<<AP_UPGRADE<<" action points ----"<<endl;
				cout<<"\n---------------\nEnter 1 if you wish to upgrade this building [or 0 to abort] \n -----> ";
				//TODO : tester niveau client si argent suffisant
				int upgrade;
				cin>>upgrade;
				if (upgrade) {
					askForBuildingUpgrade(input_);
					int upgradeResult = getConfirmation();
					if (upgradeResult) {
						cout<<"-------------\n This building has started upgrading !"<<endl;
						cout<<" Upgrade will be complete in "<<(1+buildingInfos[0])*TIMESCALE<<" minutes !"<<endl;
					}
					else cout<<"-------------\n You can't upgrade this building (not enough money or this building is already upgrading) !"<<endl;
				}
			}
			state_ = FREE;
			break;
        }

        case AP_MENU : {
            if (input_==BUY_AP){

                cout<<"Price : "<<GOLD_FOR_ONE_AP<<" gold for 1 action point"<<endl;
                cout<<"Indicate the amount of action points you wish to buy : " << flush;
                int amount;
                cin>>amount;
                buyActionPoints(amount);
                int result = getPriceForAP();
                if (result>0) cout<<" ----- You successfully bought "<<amount<<" action points for "<<result<<" gold."<<endl;
                else cout<<" ----- You don't have enough money"<<endl;
                state_=AP_MENU;
            }

            else if (input_==WAIT_FOR_AP) {
                startPromotionCampaign();
                cout<<"Press 0 to stop the promotion campaign : "<< flush;
                int end;
                cin>>end;
                if (end==0) {
                    endPromotionCampaign();
                    int APGained = getPromotionResult();
                    cout<<"You gained "<<APGained<<" from promotion"<<endl;
                    state_=FREE;
                }
            }
            else if (input_==ABORT){
                state_=FREE;
            }
            break;
        }

        case ADMIN : {
            //menu d'administration, contexte bloqué, pas de push du serveur possible dans cette configuration
            break;
        }
        default : {//ne devrait jamais passer par ici
            std::cout<<"Nothing to do here."<<std::endl; // *flies away*
        }
    }
}

void Client::askInput() {
    switch(state_) {
        case INIT : {
            displayConnexionMenu();
            break;
        }
        case FREE : {
            displayManagerInfos();
            displayMainMenu();
            break;
        }
        case MANAGERS_MENU :{
            displayAvailableManagers();
            break;
        }
        case AUCTION_MENU : {
            displayManagerInfos();
            displayAuctionMenus();
            break;
        }
        case PLAYERS_MENU : {
            displayManagerInfos();
            displayManagePlayersMenu();
            break;
        }
        case PLAYERSLIST_MENU : {
            displayPlayersList();
            cout<<"Indicate the number of the player you wish to inspect [or 0 to abort] \n -----> " << flush;
            break;
        }
        case TRAINING_MENU : {
            displayPlayersList();
            cout<<" ---- A training costs "<<AP_TRAINING<<" action points ----"<<endl;
            cout<<"Indicate the number of the player you wish to train [or 0 to abort]  \n -----> " << flush;
            break;
        }
        case HEALING_MENU : {
            displayPlayersList();
            cout<<" ---- Healing a player costs "<<AP_HOSPITAL<<" action points ----"<<endl;
            cout<<"Indicate the number of the player you wish to heal [or 0 to abort] : " << flush;
            break;
        }
        
        case BUILDINGS_MENU : {
			displayManagerInfos();
			displayManageBuildingsMenu();
			break;
		}
		
        case AP_MENU : {
            displayActionPointsMenu();
            break;
        }

        
        default : {
            std::cout<<"No options available."<<std::endl;
            break;
        }
    }
}
