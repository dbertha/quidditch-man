
#include "AuctionWindow.hpp"
#include <QTimer>


AuctionWindow::AuctionWindow(Client * client, int startingPrice, QWidget *parent)
    : QDialog(parent), startingPrice_(startingPrice), __client(client),currentTurn_(1) {
    __client->askAuctionTimeLeft();
    timeLeft_=__client->getAuctionTimeLeft();
    bidButton = new QPushButton(tr("Bid"));
    bidButton->setDefault(true);
    __client->askCurrentPrice();
    currentPrice_=__client->getCurrentPrice();
    turnPrice_=startingPrice_;
	mainLayout = new QVBoxLayout;
    QString str = QString("The current price is %1 gold").arg(currentPrice_);
    price = new QLabel(str);
    nbOfBidders_=(currentPrice_-startingPrice_)/(startingPrice_/10);
    str = QString("Bidders : %1").arg(nbOfBidders_);
    bidders = new QLabel(str);
    mainLayout->addWidget(price);
    mainLayout->addWidget(bidders);
    quitButton= new QPushButton(tr("Quit"));
    bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(bidButton);
    bottomLayout->addWidget(quitButton);
    connect(bidButton, SIGNAL(clicked()), this, SLOT(bid()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(reject()));
    mainLayout->addLayout(bottomLayout);
    //strActions<<"bid ";
  
    mainLayout->addStretch();
    setLayout(mainLayout);
    setMinimumWidth(415);
    hasBidden_=false;
    priceUpdate = new QTimer();
	connect(priceUpdate, SIGNAL(timeout()),this,SLOT(changeLabel()));
	priceUpdate->start(500);
	init();
}

void AuctionWindow::bid(){
	hasBidden_=true;
	bidButton->setEnabled(false);
	quitButton->setEnabled(false);
	__client->bid();
    int result = __client->getConfirmation();
}

void AuctionWindow::endOfTurn(){
	turnPrice_=currentPrice_;
	bidButton->setEnabled(false);
	quitButton->setEnabled(false);
	__client->checkAuction();
	int result = __client->receiveAuctionResult();



	QMessageBox * msgBox = new QMessageBox(QMessageBox::Information, "Info","End of turn", QMessageBox::Ok);
    QString msg;

	if (result<0) {
	    msg="Congrats! You have won this auction";
		if (result==-1) msg+="\n"+QString::number(currentPrice_)+" gold will be transferred in exchange of the player";
	 	else if (result==-2) {
	    	msg+="\nYou don't have enough money to pay the "+QString::number(currentPrice_)+" required";
	    	msg+=", so 3 percent of your money will be given to the owner of the player";
	    }
	    msgBox->setText(msg);
    	msgBox->exec();
    	reject();
	}
	else if (result==0) {
	    msg="Auction is now over. You have not won. Goodbye !";
	    msgBox->setText(msg);
    	msgBox->exec();
    	reject();
	}
	else {
		if (!hasBidden_) {
			msg="You've left this auction. Bye";
			msgBox->setText(msg);
	    	msgBox->exec();
	    	reject();
		}
		else {
			hasBidden_=false;
		    msg="Next turn will start soon... The current price is "+QString::number(currentPrice_);
		    msgBox->setText(msg);
		   	QTimer *msgBoxCloseTimer = new QTimer(this);
		    msgBoxCloseTimer->setInterval(5000);
		    msgBoxCloseTimer->setSingleShot(true);
		    connect(msgBoxCloseTimer, SIGNAL(timeout()), msgBox, SLOT(reject())); // or accept()
		    msgBoxCloseTimer->start();

		    msgBox->exec();
		    bidButton->setEnabled(true);
		    quitButton->setEnabled(true);
		    init();
		}
	}

}



void AuctionWindow::changeLabel(){
    __client->askCurrentPrice();
    currentPrice_=__client->getCurrentPrice();
    QString myText=QString("The current price is %1 gold").arg(currentPrice_);
    price->setText(myText);

    nbOfBidders_=(currentPrice_-turnPrice_)/(startingPrice_/10);
    myText = QString("Bidders : %1").arg(nbOfBidders_);
    bidders->setText(myText);
    update();
}


void AuctionWindow::init() {
    int timeTurn;
    if (currentTurn_==1) timeTurn=timeLeft_;
    else timeTurn = 30;
    QTimer *turn = new QTimer(this);
	turn->setInterval(timeLeft_*1000);
	turn->setSingleShot(true);
	connect(turn, SIGNAL(timeout()), this, SLOT(endOfTurn()));

  	//connect(this,SIGNAL(signal()),&thread,SLOT(updatePrice()));
  	//thread.start();
	turn->start();
}