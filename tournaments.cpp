#include "tournaments.hpp"
#include "Defines.hpp"
#include "tournamentDialog.hpp"
#include "common/NetworkInterface.hpp"

int chooseTournament(const int sockfd_, const int role, QWidget *parent) {
    QString str="A tournament starts when all places are taken.";
    QMessageBox::information(parent,QMessageBox::tr("Tournaments"),str,QMessageBox::Ok);
    int pos = NO_CHOISE;
    if (askForTournamentList(sockfd_)==0) return BAD_CONNECTION;
    std::vector<int> list;
    list = getTournamentList(sockfd_);
    QStringList items;
    char username[USERNAME_LENGTH];
    for (unsigned int i=0;i<list.size()/3; ++i) {
        items<<QString("%1 players on %2; starting price = %3").arg(list[3*i+1]).arg(list[3*i]).arg(list[3*i+2]);
    }
    TournamentDialog *selectionDialog = new TournamentDialog(sockfd_,items,role,parent);
    selectionDialog->setWindowTitle("List of tournaments");
    if (selectionDialog->exec()==selectionDialog->Accepted) {
        pos = selectionDialog->getPosition();
        std::cout<<pos+1<<"eme tournoi selectionne"<<std::endl;
    }
    return pos;
}
