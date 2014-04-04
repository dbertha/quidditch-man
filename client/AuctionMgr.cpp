#include "AuctionMgr.hpp"
#include "../common/Defines.hpp"


int chooseAuction(Client * client, QWidget *parent) {
    int pos = NO_CHOICE;
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
        AuctionsDialog *auctionsDialog = new AuctionsDialog(client,pos,atoi(auctionsList[pos*4+3].c_str()),parent);
        line=auctionsList[pos*4+1]+" - Starting price : "+auctionsList[pos*4+3];
        auctionsDialog->setWindowTitle(line.c_str());
        auctionsDialog->exec();
    }
    return pos;
}
