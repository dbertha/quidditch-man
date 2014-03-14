#include "auctionMgr.hpp"
#include "../common/Defines.hpp"


int chooseAuction(Client * client, QWidget *parent) {
    int pos = NO_CHOICE;
    if (client->getAuctionsList()==0) return BAD_CONNECTION;
    std::vector<std::string> auctionsList = client->receiveAuctionsList();
    QStringList items;
    QString qLine;
    std::string line="";
    for (unsigned int i=0;i<auctionsList.size();i+=4) {
        line+=auctionsList[i+1]+" - Starting price : "+auctionsList[i+3];
        qLine = QString::fromStdString(line);
        items.append(qLine);
        line="";
    }
    SelectionDialog *selectionDialog = new SelectionDialog(items,parent);
    selectionDialog->setWindowTitle("Select an auction");
    if (selectionDialog->exec()==selectionDialog->Accepted) {
        pos = selectionDialog->getPosition();
        std::cout<<pos+1<<"eme enchere selectionne"<<std::endl;
        //PlayersDialog *playersDialog = new PlayersDialog(client,pos,parent);
        //strncpy(username,playersList[2*pos].c_str(),USERNAME_LENGTH);
        //strcat(username," ");
        //strncat(username,playersList[2*pos+1].c_str(),USERNAME_LENGTH);
        //playersDialog->setWindowTitle(username);
        //playersDialog->exec();
    }
    return pos;
}
