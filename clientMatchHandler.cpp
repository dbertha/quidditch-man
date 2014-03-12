#include "clientMatchHandler.hpp"

int choosePartner(const int sockfd_, QWidget *parent) {
    int pos = NO_CHOISE;
    std::vector<int> IDList;
    std::vector<std::string> namesList;
    if (getManagersList(sockfd_)==0) return BAD_CONNECTION;
    receiveManagersIDandNames(sockfd_,&IDList,&namesList);
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
        int pos = selectionDialog->getPosition()+1;
        std::cout<<pos<<"eme manager selectionne"<<std::endl;
    }
    return pos;
}
