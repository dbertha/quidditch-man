#include "ClientMatchHandler.hpp"

int choosePartner(Client * client, QWidget *parent) {
    int pos = NO_CHOICE;
    std::vector<int> IDList;
    std::vector<std::string> namesList;
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
    std::vector<std::string> playersList = client->receivePlayersList();
    QStringList allNames, remainingNames, orderedRoles;
    
    char username[2*USERNAME_LENGTH+1];
    for (unsigned int i=0;i<playersList.size();i+=2) {
        strncpy(username,playersList[i].c_str(),USERNAME_LENGTH);
        strcat(username," ");
        strncat(username,playersList[i+1].c_str(),USERNAME_LENGTH);
        allNames.append(username);
    }
    orderedRoles << "KEEPER" << "SEEKER" << "CHASER1" << "CHASER2" << "CHASER3" << "BEATER1" << "BEATER2";
    remainingNames = allNames; //copie
    unsigned int i=0;
    while (i<7) {
        SelectionDialog *selectionDialog = new SelectionDialog(remainingNames,parent);
        selectionDialog->setWindowTitle("Select a " + orderedRoles[i]);
        if(selectionDialog->exec() == selectionDialog->Accepted){
            pos = selectionDialog->getPosition();
            std::cout<<pos<<"eme joueur selectionne"<<std::endl;
            PlayersDialog *playersDialog = new PlayersDialog(client,pos,0,parent);
            strncpy(username,playersList[2*pos].c_str(),USERNAME_LENGTH);
            strcat(username," ");
            strncat(username,playersList[2*pos+1].c_str(),USERNAME_LENGTH);
            playersDialog->setWindowTitle("Select "+QString(username)+" as "+orderedRoles[i]+" ?");
            if(playersDialog->exec() == playersDialog->Accepted) {
                chosenPlayersIndexes.push_back(allNames.indexOf(remainingNames[pos]));
                remainingNames.removeAt(pos); //on retire le joueur selectionne de la liste
                ++i;
            }
            else std::cout<<"mais abandonne"<<std::endl;
         }//on n'accepte pas le cancel
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

