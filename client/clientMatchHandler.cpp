#include "clientMatchHandler.hpp"

int choosePartner(Client * client, QWidget *parent) {
    int pos = NO_CHOICE;
    std::vector<int> IDList;
    std::vector<std::string> namesList;
    if (client->getManagersList()==0) return BAD_CONNECTION;
    client->receiveManagersIDandNames(&IDList,&namesList);
    QStringList items;
    char username[USERNAME_LENGTH];
    std::vector<std::string>::iterator it;
    for(it = namesList.begin(); it != namesList.end(); it++) {
        strncpy(username,(*it).c_str(),USERNAME_LENGTH);
        items.append(username);
    }
    SelectionDialog *selectionDialog = new SelectionDialog(items,parent);
    selectionDialog->setWindowTitle("Select a partner");
    if (selectionDialog->exec()==selectionDialog->Accepted) {
        pos = selectionDialog->getPosition();
        pos = IDList[pos]; //on renvoie l'ID correspondant
        std::cout<<pos<<"eme manager selectionne"<<std::endl;
    }
    return pos;
}

std::vector<int> chooseTeamForMatch(Client * client, QWidget *parent){
    int pos = NO_CHOICE;
    std::vector<int> chosenPlayersIndexes;
    if (client->askForPlayersList()==0) return chosenPlayersIndexes;
    std::vector<std::string> playersList = client->receivePlayersList();
    QStringList allNames, remainingNames;
    QStringList orderedRoles;
    
    char username[2*USERNAME_LENGTH+1];
    for (unsigned int i=0;i<playersList.size();i+=2) {
        strncpy(username,playersList[i].c_str(),USERNAME_LENGTH);
        strcat(username," ");
        strncat(username,playersList[i+1].c_str(),USERNAME_LENGTH);
        allNames.append(username);
    }
    orderedRoles << "KEEPER" << "SEEKER" << "CHASER1" << "CHASER2" << "CHASER3" << "BEATER1" << "BEATER2";
    remainingNames = allNames; //copie
    for(unsigned int i = 0; i < 7; ++i){
        SelectionDialog *selectionDialog = new SelectionDialog(remainingNames,parent);
        selectionDialog->setWindowTitle("Select the role : " + orderedRoles[i]);
        while(selectionDialog->exec() != selectionDialog->Accepted){
            SelectionDialog *selectionDialog = new SelectionDialog(remainingNames,parent);
            selectionDialog->setWindowTitle("Select the role : " + orderedRoles[i]);
        }//on n'accepte pas le cancel
        pos = selectionDialog->getPosition();
        std::cout<<pos<<"eme joueur selectionne"<<std::endl;
        chosenPlayersIndexes.push_back(allNames.indexOf(remainingNames[pos]));
        remainingNames.removeAt(pos); //on retire le joueur déjà selectionné de la liste
        
    }
    return chosenPlayersIndexes;
}

//~ int chooseTournament(Client * client, QWidget *parent){
    //~ int pos = NO_CHOICE;
    //~ 
    //~ if (client->askForTournamentList()==0) return BAD_CONNECTION;
    //~ std::vector<int> tournamentList = client->getTournamentList();
    //~ QStringList items;
    //~ 
    //~ for(unsigned int i = 0; i < tournamentList.size(); i += 3){
        //~ QString tournament = QString("Starting Number of Players : %1, Current Number of Players : %2, Starting price : %3").arg(tournamentList[i],tournamentList[i+1], tournamentList[i+2]);
        //~ items.append(tournament);
    //~ }
    //~ SelectionDialog *selectionDialog = new SelectionDialog(items,parent);
    //~ selectionDialog->setWindowTitle("Select a tournament");
    //~ if (selectionDialog->exec()==selectionDialog->Accepted) {
        //~ pos = selectionDialog->getPosition();
        //~ //pos = IDList[pos]; //on renvoie l'ID correspondant
        //~ //std::cout<<pos<<"eme manager selectionne"<<std::endl;
        //~ 
    //~ }
    //~ return pos;
//~ }

