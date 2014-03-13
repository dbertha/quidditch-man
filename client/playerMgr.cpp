#include "playerMgr.hpp"

int choosePlayer(Client * client, QWidget *parent) {
    int pos = NO_CHOICE;
    if (client->askForPlayersList()==0) return BAD_CONNECTION;
    std::vector<std::string> playersList = client->receivePlayersList();
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


    
    
/*
askForPlayerInfos(input_-1);
vector<int> playerInfos = receivePlayerInfo();
displayPlayerInfos(playerInfos,input_-1);
*/
