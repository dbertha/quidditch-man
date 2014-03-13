#include "tournamentDialog.hpp"

TournamentDialog::TournamentDialog(const int sockfd,const QStringList &items,const int role, QWidget *parent)
    : QDialog(parent), row(-1),role_(role), sockfd_(sockfd) {
    model = new QStringListModel(this);
    model->setStringList(items);
    listView = new QListView;
    listView->setModel(model);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QString str= (role==NORMAL_LOGIN) ? "Select Current" : "New";
    selectButton = new QPushButton(str);
    cancelButton= new QPushButton(tr("Cancel"));
    cancelButton->setDefault(true);
    bottomLayout = new QHBoxLayout;
    if ((items.length()>0)||(role==ADMIN_LOGIN)) {
        bottomLayout->addWidget(selectButton);
        connect(selectButton, SIGNAL(clicked()), this, SLOT(select()));
    }
    bottomLayout->addWidget(cancelButton);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(listView);
    mainLayout->addLayout(bottomLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);
}

void TournamentDialog::select() {
    QString str;
    if(role_==NORMAL_LOGIN) {
        row = listView->currentIndex().row();
        str ="Are you sure to join this tournament ?";
        if(QMessageBox::question(this,tr("Tournament"),str,
           QMessageBox::Ok | QMessageBox::Cancel)!=QMessageBox::Ok) return;
        if(askToJoinTournament(sockfd_,row)==0) reject();
        if(getConfirmation(sockfd_)) {
            str="You are recorded as a participant of this tournament.\n"
                    "Be ready for when it will start !";
            QMessageBox::information(this,tr("Tournament"),str,QMessageBox::Ok);
            accept();
        }
        else {
            str="Impossible to join this tournament !";
            QMessageBox::critical(this,tr("Tournament"),str,QMessageBox::Ok);
            reject();
        }
    }
    else {
        bool ok;
        int nb = QInputDialog::getInt(this,tr("New tournament"),
                tr("Number of players (power of 2 and not greater than 32"),2,2,32,2,&ok);
        if (ok) {
            if((not nb) or (nb & (nb - 1))) {
//http://stackoverflow.com/questions/600293/how-to-check-if-a-number-is-a-power-of-2
                str="Number of players is not a power of 2.";
                QMessageBox::critical(this,tr("Tournament"),str,QMessageBox::Ok);
            }
            else {
                int price = QInputDialog::getInt(this,tr("New tournament"),
                        tr("Enter the prize-money for the first level \n"
                           "(next levels' prize-moneys will be calculated automatically"),
                                                 10000,0,99999999,1000,&ok);
                if(ok) {
                    if(sendTournamentCreation(sockfd_,nb,price)==0) reject();
                    if (getConfirmation(sockfd_)) {
                        str="Tournament created !";
                        QMessageBox::information(this,tr("New tournament"),str,QMessageBox::Ok);
                        accept();
                    }
                    else {
                        str="Impossible to create this tournament !";
                        QMessageBox::critical(this,tr("Tournament"),str,QMessageBox::Ok);
                    }
                }
            }
        }
    }
}
int TournamentDialog::getPosition() {return row;}
void TournamentDialog::technicalProblem() {
    QString str="Technical problem\nThe server did not repond quite well";
    QMessageBox::critical(this,tr("Tournament"),str,QMessageBox::Ok);

}
