#include "TerminalGame.hpp"

using namespace std;

int timeTurn; //global
bool hasChosen;
bool isBidder;
int currentPrice;


TerminalGame::TerminalGame(int sockfd) : _client(sockfd), state_(INIT) {
    //mainloop();
}

int TerminalGame::mainLoop() {
    while(state_!=DISCONNECTING) {
        askInput();
        loadFDSet();
        if (select(_client.getSockfd()+1,&FDSet_,NULL,NULL,NULL)==ERROR) {
            std::cerr<<"Socket bind error"<<std::endl;
            return EXIT_FAILURE;
        }
        if(keyboard()) kbMgr(); // si entrée clavier
        else commMgr(); //sinon, message du serveur
    }
    return EXIT_SUCCESS;
}

void TerminalGame::askInput() {
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
        case ADMIN : {
            displayAdminMenu();
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
        case AVAILABLE : { //back to the menu without direct request to server
            displayMainMenu();
            state_ = FREE;
            break;
        }
        case BUILDINGS_MENU : {
			displayManagerInfos();
			displayManageBuildingsMenu();
			break;
		}
        case TOURNAMENTS_MENU : {
            displayTournamentMenu();
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

///////Display\\\\\\\

//Main\\

void TerminalGame::displayManagerInfos() {
    int nbPlayers, money, nbFans, actionPoints;
    _client.receiveManagerInfos(&nbPlayers,&money,&nbFans,&actionPoints);
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<" ------ MONEY : "<<money<<" ||| "<<" NUMBER OF FANS : "<<nbFans<<" ||| "<<" ACTION POINTS : "<<actionPoints<<" ------ "<<endl;
}

void TerminalGame::displayMainMenu(){
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<"What do you want to do ?"<<endl;
    cout<<" [1] See managers connected to propose match"<<endl;
    cout<<" [2] See/create auctions"<<endl;
    cout<<" [3] Manage players"<<endl;
    cout<<" [4] Manage buildings"<<endl;
    cout<<" [5] Get action points"<<endl;
    cout<<" [6] See open tournaments"<<endl;
    cout<<" [7] Play a training match"<<endl;
    cout<<" [0] Quit game"<<endl;
    cout<<"-----> " << flush;
}

void TerminalGame::displayAdminMenu(){
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<"You are the administrator"<<endl;
    cout<<"What do you want to do ?"<<endl;
    cout<<" [1] See current tournaments "<<endl;
    cout<<" [2] Create a tournament"<<endl;
    cout<<" [0] Quit game"<<endl;
    cout<<"-----> " << flush;
}

//Log in\\

void TerminalGame::displayConnexionMenu(){
    cout<<" ------------------------ WELCOME TO QUIDDITCH MANAGER 2014 ! ------------------------"<<endl;
    cout<<" [1] Log in"<<endl;;
    cout<<" [2] Create a new manager"<<endl;
    cout<<"-----> " << flush;
}

//Management\\

void TerminalGame::displayManagePlayersMenu(){
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<"//MANAGE TEAM// What do you want to do ?"<<endl;
    cout<<" [1] Inspect a player"<<endl;
    cout<<" [2] Train a player"<<endl;
    cout<<" [3] Send a player to the hospital"<<endl;
    cout<<" [0] Return to the main menu"<<endl;
    cout<<"-----> " << flush;
}

void TerminalGame::displayManageBuildingsMenu(){
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

void TerminalGame::displayActionPointsMenu(){
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<"//GET ACTION POINTS// What do you want to do ?"<<endl;
    cout<<" [1] Buy action points"<<endl;
    cout<<" [2] Start a promotion campaign"<<endl;
    cout<<" [0] Return to the main menu"<<endl;
    cout<<"-----> " << flush;  
}

void TerminalGame::displayPlayersList() {
    vector<string> playersList = _client.receivePlayersList();
    for (unsigned int i=0;i<playersList.size();i+=2) {cout<<" ["<<(i+2)/2<<"] "<<playersList[i]<<" "<<playersList[i+1]<<endl;}
}

void TerminalGame::displayBuildingInfos(std::vector<int> buildingInfos, int buildingID){
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

void TerminalGame::displayPlayerInfos(vector<int> playerInfos, int playerID) {
    std::vector<string> playersList = _client.receivePlayersList();
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

void TerminalGame::displayAuctionMenus(){
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<"//YOU'RE NOW IN THE AUCTION ROOM// What do you want to do ?"<<endl;
    cout<<" [1] See current auctions"<<endl;
    cout<<" [2] Sell a player"<<endl;
    cout<<" [0] Return to the main menu"<<endl;
    cout<<"-----> " << flush;
}

void TerminalGame::displaySellPlayerMenu(){
    cout<<" --------------------------------------------------------------------------------------"<<endl;
    cout<<"//YOU'RE ABOUT TO SELL A PLAYER// What do you want to do ?"<<endl;
    displayPlayersList();
    cout<<" ---- Selling a player costs "<<AP_AUCTION<<" action points ----"<<endl;
    cout<<" Which player do you want to sell [enter 0 to abort] ? \n-----> "<<endl;
}

void TerminalGame::displayAvailableManagers(){
    std::vector<int> IDList;
    std::vector<std::string> namesList;
    _client.receiveManagersIDandNames(&IDList,&namesList);
    for(unsigned int i = 0; i < IDList.size(); ++i){
        std::cout << "ID :" << IDList[i] << " name : " << namesList[i] << std::endl;
    }
    cout<<"Indicate the ID of the player you want to challenge [-1 to go back]: " << flush;
}

void TerminalGame::displayTournamentMenu(){
    std::vector<int> tournamentList = _client.getTournamentList();
    displayTournamentList(tournamentList);
    //~ cout<<"Indicate the ID of the tournament you want to join " << \
    //~ "(you can't join several tournaments at the same time) [-1 to go back]: " << flush;
    if(tournamentList.size() > 0) cout<<"Do you wanna join this tournament ? [-1 for no, other number for yes]: " << flush;
    else{
        cout<<"No tournament available." << endl;
        state_ = FREE;
        displayMainMenu();
    }
}



std::vector<int> TerminalGame::displayAndAskPlayersForMatch(){
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



std::string TerminalGame::displayAuctionInfos(vector<string> auctionsList,vector<int> playerInfos, int auctionID) {
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

void TerminalGame::displayAuctionsList(vector<string> auctionsList) {
    if (auctionsList.size()==0) cout<<" --------- No auction available"<<endl;
    else {
        for (unsigned int i=0;i<auctionsList.size();i+=4) {
            cout<<" ["<<atoi(auctionsList[i].c_str())+1<<"] "<<auctionsList[i+1]<<" | Starting price : "<<auctionsList[i+3];
            cout<<" | Time left to participate : "<<auctionsList[i+2]<<endl;
        }
        cout<<"Indicate which auction you wish to inspect [or 0 to abort] : ";
    }
}



//Match\\

void TerminalGame::displayTournamentList(std::vector<int> tournamentsList){
    std::cout << "A tournament starts when all places are taken" << std::endl;
    for(unsigned int i = 0; i < tournamentsList.size(); i += 3){
        std::cout << "Starting Number of Players : " << tournamentsList[i] << std::endl;
        std::cout << "Current Number of Players : " << tournamentsList[i+1] << std::endl;
        std::cout << "Starting price : " << tournamentsList[i+2] << std::endl;
    }
}


void TerminalGame::startMatch(int numTeam){
    int winner = 0;
    int scoreTeam1 = 0;
    int scoreTeam2 = 0;
    HexagonalField field;
    std::vector<AxialCoordinates> allPositions;
    allPositions = _client.receiveScoresAndPositions(&winner, &scoreTeam1, &scoreTeam2);
    cout << "Your are team n° " << numTeam << endl;
    while(winner == 0){
        winner = testifContinue(numTeam); //demande match nul ou forfait + écoute socket pour match nul ou socket
        if(winner == 0){ 
            cout << "Your are team n° " << numTeam << endl;
            cout << "Score team1 (inviter) = " << scoreTeam1 << endl;
            cout << "Score team2 (invited) = " << scoreTeam2 << endl;
            field.reset();
            for(unsigned int i = 0; i < allPositions.size(); ++i){
                
                if((allPositions[i].getDiagAxis() != 10000) and (field.getOccupant(allPositions[i]) == FREE_SPACE)){
                
                    field.setOccupant(allPositions[i], i);
                
                }else if(allPositions[i].getDiagAxis() == 10000){
                    int playerRole = i;
                    int playerTeam = 1;
                    if(i > TEAM2_KEEPER){
                        playerTeam = 2;
                        playerRole = i - TEAM2_KEEPER;
                    }
                    std::cout << "Le joueur " << playerRole << "de l'équipe " << playerTeam << " a été abattu par les cognards." << std::endl;
                }else{
                    int playerRole = field.getOccupant(allPositions[i]);
                    int playerTeam = 1;
                    if(playerRole > TEAM2_KEEPER){
                        playerTeam = 2;
                        playerRole = playerRole - TEAM2_KEEPER;
                    }
                    std::cout << "Le joueur " << playerRole << "de l'équipe " << playerTeam << " possède le souaffle." << std::endl;
                }
            }
            field.display();
            askAndSendMoves(numTeam, field, allPositions);
            _client.getConfirmation();
            
            //cout << "Les échanges de messages suivants pour le match n'ont pas encore été implémentés." << endl;
            //winner =1;
            allPositions = _client.receiveScoresAndPositions(&winner, &scoreTeam1, &scoreTeam2);
        }
    }
    cout << "Winner is team " << winner << " (-1 is a draw)"<< endl;
}

int TerminalGame::testifContinue(int numTeam){
    int winner = 0;
    cout << "You are team n° :  " << numTeam << endl;
    cout << "You are in a match, what do you want to do :  " << endl;
    cout << "[1] Make next turn " << endl;
    cout << "[2] Forfeit "  << endl;
    cout << "[3] Suggest a draw (in a tournament, the one who suggest is considered as the loser) " << endl;
    loadFDSet();
    if (select(_client.getSockfd()+1,&FDSet_,NULL,NULL,NULL)==ERROR) {
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
                _client.sendForfeit(); //pas besoin d'une confirmation
                return numTeam == 1 ? 2 : 1; //team adverse gagne
            }
            case 3 : {
                _client.sendDrawRequest();
                int result = _client.getConfirmation();
                if(result == DRAWACCEPTED){
                    cout << "Draw accepted by the opponent ! " << endl;
                    return -1;
                }else{
                    cout << "Draw refused ! " << endl;
                    return winner;
                }
            }
        }
    }
    else{ //sinon, message du serveur
        SerializedObject received = receiveOnSocket(_client.getSockfd());
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
                _client.sendAnswerToDrawProposition(code);
                return input_ ? -1 : 0;
            }
        }
    }
    return winner;
}



void TerminalGame::askAndSendMoves(int numTeam, HexagonalField &field, std::vector<AxialCoordinates> &positions){
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
      attributs = _client.receiveSelectedPlayerInfos(selectedPlayerID);
      field.display(attributs.position, attributs.attributes[SPEED]);
      std::cout << "Attributs du joueur sélectionné  : " << std::endl;
      std::cout << "Vitesse  : " << attributs.attributes[SPEED] << std::endl;
      std::cout << "Force  : " << attributs.attributes[STRENGTH] << std::endl;
      std::cout << "Précision  : " << attributs.attributes[PRECISION] << std::endl;
      std::cout << "Réflexe  : " << attributs.attributes[REFLEX] << std::endl;
      std::cout << "Résistance  : " << attributs.attributes[RESISTANCE] << std::endl;
      std::cout << "Vie restante  : " << attributs.life << std::endl;
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
  //Amélioration possible : tester si reception d'une demande de match nul ou forfait
  _client.sendMoves(moves);
}

////////handling interactions :\\\\\\\



void TerminalGame::loadFDSet() {
//charge le set des FD que doit surveiller en lecture accept()
//comme select() écrase cet input par la liste des FD actifs,
//on exécute cette fonction avant chaque select()
    FD_ZERO(&FDSet_); //l'ensemble est vidé
    FD_SET(_client.getSockfd(),&FDSet_); //le socket
    FD_SET(STDIN_FILENO,&FDSet_); //le file descriptor de l'input pour accepter des commandes du clavier
}

bool TerminalGame::keyboard() {return FD_ISSET(STDIN_FILENO,&FDSet_);}


void TerminalGame::handleLogin(){
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
    std::cout<<"Enter login : ";
    std::cin>>username;
    std::cout<<"Enter password : ";
    std::cin>>password;
    if (input_==LOG_IN) _client.sendLoginToServer(username,password);
    else if (input_==NEW_MANAGER) _client.sendNewManagerToServer(username,password);
    int result = _client.getConfirmation();
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
#ifdef __DEBUG
            cout << "You are in admin mode." << endl;
#endif
        }
    }
}

void TerminalGame::handleMainMenu(){
    

    switch(input_){
//Rappel :
//#define SEE_MANAGERS 1
//#define AUCTION_ROOM 2
//#define MANAGE_PLAYERS 3
//#define MANAGE_BUILDINGS 4
//#define SEE_TOURNAMENTS 5
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
        case SEE_TOURNAMENTS : {
            state_ = TOURNAMENTS_MENU;
            break;
        }
        case START_TRAINING_MATCH : {
            std::vector<int> team = displayAndAskPlayersForMatch();
            _client.sendTrainingMatchRequest(team);
            if(_client.getConfirmation() == 1){
                startMatch(1);
            }
            state_ = FREE;
            break;
        }
        default : {
			state_ = DISCONNECTING;
            break;
		}
        

    }
}

void TerminalGame::handleAdminMenu(){
    switch(input_){
        case SEE_TOURNAMENTS_ADMIN : {
            std::vector<int> tournamentsList;
            tournamentsList = _client.getTournamentList();
            displayTournamentList(tournamentsList);

            break;
        }
        case CREATE_TOURNAMENT_OPTION : {
            int nbOfPlayers, startingPrice, result;
            cout << "Please choose the number of participants wanted "<<
            "(the tournament will only start when enough suscribers). "<<
            "It must be a power of two : " << endl;
            cin >> nbOfPlayers;
            if(not ((not nbOfPlayers) or (nbOfPlayers & (nbOfPlayers - 1)))) { //if power of two
                cout << "Please choose the prize-money for the first level "<<
                "(next levels' prize-moneys will be calculated automatically) : " << endl;
                cin >> startingPrice;
                _client.sendTournamentCreation(nbOfPlayers, startingPrice);
                result = _client.getConfirmation();
                if(result == 0){
                    cout << "Creation failed, maybe there is still an active tournament"  << endl;
                }else{
                    cout << "Tournament created ! " << endl;
                }
                              
            }
            break;
        }
        default : {
			state_ = DISCONNECTING;
			break;
		}
        
    }
}

void TerminalGame::handleOpponentChoice(){
    if(input_ == -1){
        state_ = FREE;
        return;
    }
    //TODO : tester input
    //verrou du contexte après le choix d'un adversaire
    std::vector<int> playersInTeam = displayAndAskPlayersForMatch();
    _client.proposeMatchTo(input_,  playersInTeam);
    if(_client.receiveMatchConfirmation() == MATCH_STARTING){
        startMatch(1); //inviteur a l'équipe 1
    }
    state_=FREE;
}

void TerminalGame::handleAuctions(){
    if (input_==SEE_AUCTIONS) {
        vector<string> auctionsList = _client.receiveAuctionsList();
        displayAuctionsList(auctionsList);
        if (auctionsList.size()!=0) {
            int auctionToInspect;
            cout<<" -----> ";
            cin>>auctionToInspect;
            if (auctionToInspect!=0) {
                vector<int> playerAuctionInfos = _client.receivePlayerInfo(auctionToInspect-1);
                string timeLeft = displayAuctionInfos(auctionsList,playerAuctionInfos,auctionToInspect-1);
                cout<<" ---- Entering an auction costs "<<AP_ENTER_AUCTION<<" action points ----"<<endl;
                cout<<"Do you want to join this auction ? [1 to enter, 0 to quit] \n -----> ";
                int enterAuction;
                cin>>enterAuction;
                if (enterAuction==1) {
                    _client.joinAuction(auctionToInspect-1);
                    int joinResult = _client.getConfirmation();
                    if (joinResult==1) {
                        _client.askAuctionTimeLeft();
                        int auctionTimeLeft=_client.getAuctionTimeLeft();
                        mainAuction(auctionToInspect-1,auctionTimeLeft);
                    }
                    else cout<<" ---- You don't have enough action points to join this auction"<<endl;
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
                vector<int> playerInfos = _client.receivePlayerInfo(sellPlayerChoice-1);
                displayPlayerInfos(playerInfos,sellPlayerChoice-1);
                cout<<"Indicate the starting price of the auction [or 0 to abort] \n -----> ";
                int startingPrice;
                cin>>startingPrice;
                if (startingPrice>0) {
                    _client.sellPlayer(sellPlayerChoice-1,startingPrice);
                    bool sellingResult = _client.getConfirmation(); //always true
                    if (!sellingResult) cout<<" ----- The player is blocked or you don't have enough action points"<<endl;
                    else cout<<" ----- Auction started !"<<endl;
                    sellPlayerChoice=ABORT;
                }
            }
        } while (sellPlayerChoice!=ABORT);
    }
    else state_ = FREE;
}

void TerminalGame::handlePlayersMenu(){
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

void TerminalGame::kbMgr() {
    cin>>input_;
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
        case ADMIN : {
            //verrouilage du contexte
            handleAdminMenu();
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
                vector<int> playerInfos = _client.receivePlayerInfo(input_-1);
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
				  _client.trainPlayer(input_-1,capacityNumber-1);
				  bool trainingResult = _client.getConfirmation();
				  if (trainingResult) {
					vector<int> trainingCenterInfos = _client.receiveBuildingInfos(TRAININGCENTER);
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
                _client.healPlayer(input_-1);
                int healResult = _client.getConfirmation();
                if (healResult){
                    vector<int> hospitalInfos = _client.receiveBuildingInfos(HOSPITAL);
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
				vector<int> buildingInfos = _client.receiveBuildingInfos(input_);
				displayBuildingInfos(buildingInfos,input_);
                cout<<"\n ---- Upgrading a building costs "<<AP_UPGRADE<<" action points ----"<<endl;
				cout<<"\n---------------\nEnter 1 if you wish to upgrade this building [or 0 to abort] \n -----> ";
				//TODO : tester niveau client si argent suffisant
				int upgrade;
				cin>>upgrade;
				if (upgrade) {
					_client.askForBuildingUpgrade(input_);
					int upgradeResult = _client.getConfirmation();
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
                _client.buyActionPoints(amount);
                int result = _client.getPriceForAP();
                if (result>0) cout<<" ----- You successfully bought "<<amount<<" action points for "<<result<<" gold."<<endl;
                else cout<<" ----- You don't have enough money"<<endl;
                state_=AP_MENU;
            }

            else if (input_==WAIT_FOR_AP) {
                _client.startPromotionCampaign();
                cout<<"Press 0 to stop the promotion campaign : "<< flush;
                int end;
                cin>>end;
                if (end==0) {
                    _client.endPromotionCampaign();
                    int APGained = _client.getPromotionResult();
                    cout<<"You gained "<<APGained<<" from promotion"<<endl;
                    state_=FREE;
                }
            }
            else state_=FREE;
            break;
        }

        case TOURNAMENTS_MENU : {
            int confirmation;
            
            if(input_ == -1){
                state_ = FREE;
                return;
            }
            //TODO : tester input ?
            _client.askToJoinTournament();
            confirmation = _client.getConfirmation();
            if(confirmation == 0){
                cout << "Impossible to join this tournament !" << endl;
            }else{
                cout << "You are recorded as a participant of this tournament. " << \
                "Be ready for when it will start !" << endl;
            }
            state_ = AVAILABLE;
            break;
        }
        default : {//ne devrait jamais passer par ici
            std::cout<<"Nothing to do here."<<std::endl; // *flies away*
        }
    }
}

void TerminalGame::commMgr() {
//gère les messages non sollicités (exemple : invitation à un match amical)
	SerializedObject received = receiveOnSocket(_client.getSockfd());
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
            _client.answerMatchProposal(confirmation, playersInTeam); //liste vide = refus de l'invitation
            if(_client.receiveMatchConfirmation() == MATCH_STARTING){
                startMatch( 2); //invité a l'équipe 2
            }
            state_ = FREE;
            break;
        }
        case SERVER_DOWN : {
            state_ = DISCONNECTING;
            cout << "Server is down." << endl;
            break;
        }
        case MATCH_TOURNAMENT_START : {
            int IDOpponent, numTeam;
            char name[USERNAME_LENGTH];
            char * position = received.stringData;
            cout << "A tournament turn starts now !" << endl;
            memcpy(&IDOpponent, position, sizeof(IDOpponent));
            position += sizeof(IDOpponent);
            memcpy(&name, position, sizeof(name));
            position += sizeof(name);
            
            cout << "Opponent ID : " << IDOpponent << " name : " << name << endl;
            //forced to accept
            std::vector<int> playersInTeam;
            playersInTeam = displayAndAskPlayersForMatch();
            _client.sendTeamForMatchTournament(playersInTeam);
            numTeam = _client.receiveNumOfTeam();
            if(numTeam > 0){
                startMatch(numTeam);
            }
            state_ = AVAILABLE;
            break;
        }
	}
}










void TerminalGame::mainAuction(int auctionID, int timeLeft) {
  hasChosen=false;
  isBidder=false;
  timeTurn=timeLeft;
  int turn = 0;
  currentPrice=0;
  _client.askCurrentPrice();
  currentPrice = _client.getCurrentPrice();
  pthread_t thread,thread2;
  int res = pthread_create(&thread, NULL, auctionTurn, (void*) &_client);
  int res2 = pthread_create(&thread2, NULL, auctionPrice, (void*) &_client);
  sleep(timeLeft);
  pthread_cancel(thread2);
  pthread_cancel(thread);
  sleep(2);
  _client.checkAuction();
  int result = _client.receiveAuctionResult();
  cout << "result : " << result << " price " << currentPrice << endl;
  if (result<0) {
    cout<<" -Congrats! You have won this auction"<<endl;
    if (result==-1) cout<<" --- "<<currentPrice<<" gold will be transferred in exchange of the player"<<endl;
    else if (result==-2) {
      cout<<" --- You don't have enough money to pay the "<<currentPrice<<" required";
      cout<<", so 3 percent of your money will be given to the owner of the player"<<endl;
    }
  }
  else if (result==0) {
    cout<<" -Auction is now over. You have not won. Goodbye !"<<endl;
  }
  else {
    cout<<" -Next turn will start soon... The current price is "<<currentPrice<<endl;
    sleep(8);
    turn+=1;
    timeTurn=30;
    while ((result>0)&&(hasChosen)&&(isBidder)) {
        //startAuctionTurn();
        //getConfirmation();
        hasChosen=false;
        res = pthread_create(&thread, NULL, auctionTurn, (void*) &_client);
        res2 = pthread_create(&thread2, NULL, auctionPrice, (void*) &_client);
        sleep(30);
        pthread_cancel(thread2);
        pthread_cancel(thread);
        sleep(2);
        cout<<" --- END OF TURN"<<endl;
        _client.checkAuction();
        result = _client.receiveAuctionResult();
        cout << "result : " << result << " price " << currentPrice << endl;
        //if ((!hasChosen)||(!isBidder)) {
        //  cout<<" -Goodbye! You have left this auction"<<endl;
        //}
        if (result<0) {
          cout<<" -Congrats! You have won this auction"<<endl;
          if (result==-1) cout<<" --- "<<currentPrice<<" gold will be transfered in exchange of the player"<<endl;
          else if (result==-2) {
            cout<<" --- You don't have enough money to pay the "<<currentPrice<<" required";
            cout<<", so 3 percent of your money will be given to the owner of the player"<<endl;
          }
        }
        else if (result==0) {
            cout<<"Sorry, you've lost this auction"<<endl;
        }
        else {
            if ((!hasChosen)||(!isBidder)) {
                cout<<" -Goodbye! You have left this auction"<<endl;
            }
            else {
              cout<<" -Next turn will start soon... The current price is "<<currentPrice<<endl;
              sleep(10);
            }
        }
        turn+=1;
      } 
    }
}






//non-membre pour le thread :

void *auctionTurn(void* data) {

  //int* sockfd = (int *)data;
  Client *client = (Client *) data;
    //TODO : si solution conservée, mutexes
  cout<<"Auction turn has started !"<<endl;
  cout<<" [1] Bid "<<endl;
  cout<<" [2] Quit auction"<<endl;


  int choice;
  cin>>choice;

  if (choice==1) {
    client->bid();
    int result = client->getConfirmation();
    //cout << "Bid done, waiting for the end of the turn. " << endl;
    isBidder=true;
    hasChosen=true;
  }
  else {
    isBidder=false;
    hasChosen=true;
  }

}



void *auctionPrice(void* data) {

  //int* sockfd = (int *)data;
  Client *client = (Client *) data;

  for (int tic=0;tic<timeTurn-1;++tic){
    client->askCurrentPrice();
    int result = client->getCurrentPrice();
    if (result>currentPrice) {
        cout<<"BID DONE. Price is now "<<result<<" gold"<<endl;
        currentPrice=result;
    }
    sleep(1);
  }
}


