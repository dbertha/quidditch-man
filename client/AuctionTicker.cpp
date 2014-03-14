#include "AuctionTicker.hpp"
#include "auctionWindow.hpp"
AuctionTicker::AuctionTicker(Client * client, int startingPrice, QSocketNotifier * notifier, MainGui *parent)
    : QWidget(parent), parent_(parent), __client(client), __pushesNotifier(notifier) {
    myTimerId = 0; // no timer started yet
    counter = 0;
    currentPrice = startingPrice;
    move(0,25); // position in parent window
    label = new QLabel(tr("you have"));
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(label);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);
    setFixedHeight(sizeHint().height());
    hasBidden_=false;
}
void AuctionTicker::showInfo() {
    move(0,50-counter%5); //this is to show its activity
    if(__client->askCurrentPrice()==0) {
        QErrorMessage *errorMessageDialog = new QErrorMessage(this);
        errorMessageDialog->showMessage(tr("No connection with the server."));
        close();
    }
    currentPrice= __client->getCurrentPrice();
    parent_->setCurrentPrice(currentPrice);
    myText=QString("The current price is %1 gold").arg(currentPrice);
    label->setText(myText);
    update();
    ++counter;
}

void AuctionTicker::paintEvent(QPaintEvent *) {
    QPainter painter(this);
}

void AuctionTicker::showEvent(QShowEvent *) {
    //starts the timer when the widget is visible
    showInfo();
    myTimerId = startTimer(5000);
}

void AuctionTicker::timerEvent(QTimerEvent *event) {
    //checks first it's about our event !
    if (event->timerId() == myTimerId) {
        std::cout << "Socket Notifier coupé" << std::endl;
        __pushesNotifier->setEnabled(false); //on n'écoute plus le socket car on prend l'initiative
        showInfo();
        __pushesNotifier->setEnabled(true);
        std::cout << "Socket Notifier activé" << std::endl;
    }else QWidget::timerEvent(event);
    //it's not about our event - let's give it to our base class
}

void AuctionTicker::hideEvent(QHideEvent *) {
    //timer stops when widget is hidden
    killTimer(myTimerId);
    myTimerId = 0;
}
