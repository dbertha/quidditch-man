#include "playerMgr.hpp"

int choosePlayer(const int sockfd_, QWidget *parent) {
    int pos = NO_CHOISE;
    if (askForPlayersList(sockfd_)==0) return BAD_CONNECTION;
    std::vector<std::string> playersList = receivePlayersList(sockfd_);
    QStringList items;
    char username[2*USERNAME_LENGTH+1];
    for (unsigned int i=0;i<playersList.size();i+=2) {
        strncpy(username,playersList[i].c_str(),USERNAME_LENGTH);
        strcat(username," ");
        strncat(username,playersList[i+1].c_str(),USERNAME_LENGTH);
        items.append(username);
    }
    SelectionDialog *selectionDialog = new SelectionDialog(items,parent);
    selectionDialog->setWindowTitle("Select a player");
    if (selectionDialog->exec()==selectionDialog->Accepted) {
        pos = selectionDialog->getPosition()+1;
        std::cout<<pos<<"eme joueur selectionne"<<std::endl;
    }
    return pos;
}

std::vector<int> chooseTeamForMatch(const int sockfd_, QWidget *parent){
    int pos = NO_CHOISE;
    std::vector<int> chosenPlayersIndexes;
    if (askForPlayersList(sockfd_)==0) return chosenPlayersIndexes;
    std::vector<std::string> playersList = receivePlayersList(sockfd_);
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
    
    
/*
askForPlayerInfos(input_-1);
vector<int> playerInfos = receivePlayerInfo();
displayPlayerInfos(playerInfos,input_-1);
*/
