#include "Tournaments.hpp"
#include "Defines.hpp"


int chooseTournament(Client * client, const int role, QWidget *parent) {
    QString _str="A tournament starts when all places are taken.";
    QMessageBox::information(parent,QMessageBox::tr("Tournaments"),_str,QMessageBox::Ok);
    int _pos = NO_CHOICE;
    std::vector<int> _list;
    _list = client->getTournamentList();
    QStringList items;
    char username[USERNAME_LENGTH];
    for (unsigned int i=0;i<_list.size()/3; ++i) {
        items<<QString("%1 players on %2; starting price = %3").arg(_list[3*i+1]).arg(_list[3*i]).arg(_list[3*i+2]);
    }
    TournamentDialog *_selectionDialog = new TournamentDialog(client,items,role,parent);
    _selectionDialog->setWindowTitle("List of tournaments");
    if (_selectionDialog->exec()==_selectionDialog->Accepted) {
        _pos = _selectionDialog->getPosition();
        std::cout<<_pos+1<<"eme tournoi selectionne"<<std::endl;
    }
    return _pos;
}
