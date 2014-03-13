#include "ticker.hpp"
#include "mainGui.hpp"
Ticker::Ticker(Client * client, MainGui *parent)
    : QWidget(parent), parent_(parent), __client(client) {
    myTimerId = 0; // no timer started yet
    counter =nbPlayers=money=nbFans=actionPoints=0;
    move(0,25); // position in parent window
    label = new QLabel(tr("you have"));
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(label);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);
    setFixedHeight(sizeHint().height());
}
void Ticker::showInfo() {
    move(0,50-counter%5); //this is to show its activity
    if(__client->askForManagerInfos()==0) {
        QErrorMessage *errorMessageDialog = new QErrorMessage(this);
        errorMessageDialog->showMessage(tr("No connection with the server."));
        close();
    }
    __client->receiveManagerInfos(&nbPlayers,&money,&nbFans,&actionPoints);
    parent_->setMoney(money);
    parent_->setNbPlayers(nbPlayers);
    parent_->setNbPlayers(nbPlayers);
    parent_->setActionPoints(actionPoints);
    myText=QString("You have : MONEY : %1 - NUMBER OF FANS : %2 - PLAYERS : %3 - ACTION POINTS : %4").arg(money).arg(nbFans).arg(nbPlayers).arg(actionPoints);
    label->setText(myText);
    update();
    ++counter;
}

void Ticker::paintEvent(QPaintEvent *) {
    QPainter painter(this);
}

void Ticker::showEvent(QShowEvent *) {
    //starts the timer when the widget is visible
    showInfo();
    myTimerId = startTimer(5000);
}

void Ticker::timerEvent(QTimerEvent *event) {
    //checks first it's about our event !
    if (event->timerId() == myTimerId) showInfo();
    else QWidget::timerEvent(event);
    //it's not about our event - let's give it to our base class
}

void Ticker::hideEvent(QHideEvent *) {
    //timer stops when widget is hidden
    killTimer(myTimerId);
    myTimerId = 0;
}
