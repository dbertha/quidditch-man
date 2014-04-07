#include "AuctionWidget.hpp"
#include "OfficePage.hpp"

AuctionWidget::AuctionWidget(Client* client, QWidget* parent) : _client(client),_parent(parent){
	
	setStyleSheet("AuctionWidget{ background-color: rgba(255, 255, 255, 100);}");
	_currentTurn=1;
	_listAuctionsWidget = new ListAuctionsWidget(_client,this);


	_joinButton= new QPushButton(tr("Join"));
	_bidButton = new QPushButton(tr("Bid"));
	_quitButton = new QPushButton(tr("Quit"));
	_joinButton->setEnabled(false);
	_joinButton->setVisible(false);
	_joinButton->setMaximumWidth(45);
	_bidButton->setMaximumWidth(45);
	_quitButton->setMaximumWidth(45);



	_priceLabel = new QLabel(tr("Current price : %1").arg(_currentPrice));
	_biddersLabel = new QLabel(tr("Bidders :%1").arg(_nbOfBidders));
	_priceLabel->setStyleSheet("QLabel{font: 18px \"Elegant Thin\", sans-serif;color: #53502d;}");
	_biddersLabel->setStyleSheet("QLabel{font: 18px \"Elegant Thin\", sans-serif;color: #53502d;}");


	_stack = new QStackedWidget();
	_errorLabel = new QLabel("Player blocked or not enough action points");
	_errorLabel->setVisible(false);

    QWidget *panel = new QWidget(this);
	 
	QGridLayout *grid = new QGridLayout(this);
	panel->setLayout(grid);
	

	grid->setRowMinimumHeight(0,340);
	grid->setRowMinimumHeight(1,30);
	grid->setRowMinimumHeight(2,30);
	grid->setColumnMinimumWidth(0,135);
	grid->setColumnMinimumWidth(1,70);
	grid->setColumnMinimumWidth(2,45);
	grid->setColumnMinimumWidth(3,45);
	grid->setColumnMinimumWidth(4,45);

	grid->setVerticalSpacing(0);
	grid->setHorizontalSpacing(0);

	grid->addWidget(_listAuctionsWidget,0,0,1,5,Qt::AlignCenter);
	grid->addWidget(_priceLabel,1,0,Qt::AlignLeft);
	grid->addWidget(_biddersLabel,1,1,Qt::AlignLeft);
	grid->addWidget(_bidButton,1,2,Qt::AlignRight);
	grid->addWidget(_quitButton,1,3,Qt::AlignLeft);
	grid->addWidget(_joinButton,1,4,Qt::AlignLeft);
	grid->addWidget(_errorLabel,2,0,1,5,Qt::AlignCenter);

	setFixedSize(520,440);

	_endOfTurnTimer = new QTimer();
	_nextTurnTimer = new QTimer();
	_updater = new QTimer();
	_endOfTurnTimer->setInterval(1000);
	_nextTurnTimer->setInterval(1000);
	_updater->setInterval(1000);
	connect(_endOfTurnTimer, SIGNAL(timeout()), this, SLOT(prepareForEndOfTurn()));
	connect(_nextTurnTimer, SIGNAL(timeout()), this, SLOT(prepareForNextTurn()));
	connect(_updater,SIGNAL(timeout()),this,SLOT(changeLabel()));
	connect(_joinButton,SIGNAL(clicked()),this,SLOT(join()));
    connect(_bidButton, SIGNAL(clicked()), this, SLOT(bid()));
    connect(_quitButton, SIGNAL(clicked()), this, SLOT(quit()));

    init();

}
void AuctionWidget::init(){

	dynamic_cast<OfficePage*>(_parent)->deblockButtons();
	_listAuctionsWidget->update();
	_listAuctionsWidget->show();
	_listAuctionsWidget->resume();
	_updater->stop();
	_endOfTurnTimer->stop();
	_nextTurnTimer->stop();
	_joinButton->setVisible(true);
	_bidButton->setEnabled(false);
	_bidButton->setVisible(false);
	_quitButton->setEnabled(false);
	_quitButton->setVisible(false);
	_nbOfBidders=0;
	_currentPrice=0;
	_priceLabel->setText(tr("Current price : %1").arg(_currentPrice));
	_biddersLabel->setText(tr("Bidders : %1").arg(_nbOfBidders));
	_priceLabel->setVisible(false);
	_biddersLabel->setVisible(false);
	_currentTurn=1;
	_hasBidden=false;
	_hasLeft=false;
}

void AuctionWidget::makeJoinable(){
	_auctionID = _listAuctionsWidget->getAuctionID();
	if (_auctionID==-1) {
		_joinButton->setVisible(false);
		_joinButton->setEnabled(false);
	}
	else {
		_joinButton->setEnabled(true);
		_joinButton->setVisible(true);
	}
}

void AuctionWidget::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void AuctionWidget::pause() {_listAuctionsWidget->pause();}
void AuctionWidget::resume() {_listAuctionsWidget->resume();}

void AuctionWidget::join() {
	_auctionID = _listAuctionsWidget->getAuctionID();
	if (_auctionID!=-1){
	    _client->joinAuction(_auctionID);
	    int joinResult = _client->getConfirmation();
	    if (joinResult==1){
	    	dynamic_cast<OfficePage*>(_parent)->blockButtons();
	    	startTurn();
	    }
	    else {
	    }
	}
}
void AuctionWidget::bid() {

	_hasBidden=true;
	_bidButton->setEnabled(false);
	_quitButton->setEnabled(false);
	_client->bid();
    _client->getConfirmation();

}

void AuctionWidget::quit() {
	_bidButton->setEnabled(false);
	_quitButton->setEnabled(false);
	_hasLeft=true;
}

void AuctionWidget::endOfTurn() {
	++_currentTurn;
	_updater->stop();
	_turnPrice=_currentPrice;
	_bidButton->setEnabled(false);
	_quitButton->setEnabled(false);
	_client->checkAuction();
	int result = _client->receiveAuctionResult();

	if (result<0) {
		if (result==-1) {
			_errorLabel->setText(tr("Auction won. %1 gold to pay").arg(_currentPrice));
			update();
			QTimer * timer = new QTimer();
			timer->setInterval(5000);
			timer->setSingleShot(true);
			connect(timer, SIGNAL(timeout()), this, SLOT(init()));
			timer->start();
		}
	 	else if (result==-2) {
	    	_errorLabel->setText(tr("Auction won. Not enough money. Penality to pay"));
	    	update();
			QTimer * timer = new QTimer();
			timer->setInterval(5000);
			timer->setSingleShot(true);
			connect(timer, SIGNAL(timeout()), this, SLOT(init()));
			timer->start();
	    }
	    /*
	    msgBox->setText(msg);
    	msgBox->exec();
    	reject();
    	*/
	}
	else if (result==0) {
	    _errorLabel->setText(tr("Auction is now over. You have not won. Goodbye !"));
	    update();
		QTimer * timer = new QTimer();
		timer->setInterval(5000);
		timer->setSingleShot(true);
		connect(timer, SIGNAL(timeout()), this, SLOT(init()));
		timer->start();
	}
	else {
		if (!_hasBidden) {
			_errorLabel->setText(tr("You've left this auction. Bye"));
			update();
			QTimer * timer = new QTimer();
			timer->setInterval(5000);
			timer->setSingleShot(true);
			connect(timer, SIGNAL(timeout()), this, SLOT(init()));
			timer->start();
		    
		}
		else {
			_hasBidden=false;
		    _errorLabel->setText(tr("Next turn will start soon"));
		    update();
		   	_timeBeforeTurn=8;
			_nextTurnTimer->start();
		}
	}
	_errorLabel->setVisible(true);
}

void AuctionWidget::prepareForNextTurn(){
	--_timeBeforeTurn;
	if (_timeBeforeTurn>1) _errorLabel->setText(tr("Next turn will start in %1 seconds").arg(_timeBeforeTurn));
	else _errorLabel->setText(tr("Next turn will start in %1 second").arg(_timeBeforeTurn));
	update();
	if (_timeBeforeTurn==0) {
		_nextTurnTimer->stop();
		_bidButton->setEnabled(true);
		_quitButton->setEnabled(true);
		startTurn();
	}

}

void AuctionWidget::prepareForEndOfTurn(){
	--_timeBeforeEndOfTurn;
	if (_timeBeforeEndOfTurn>1) _errorLabel->setText(tr("Turn ends in %1 seconds").arg(_timeBeforeEndOfTurn));
	else _errorLabel->setText(tr("Turn ends in %1 second").arg(_timeBeforeEndOfTurn));
	update();
	if (_timeBeforeEndOfTurn==0) {
		if (_hasLeft==true) {
			QTimer * timer = new QTimer();
			timer->setInterval(5000);
			timer->setSingleShot(true);
			connect(timer, SIGNAL(timeout()), this, SLOT(init()));
			timer->start();
		}
		_timeBeforeEndOfTurn=30;
		_endOfTurnTimer->stop();
		endOfTurn();
	}
}


void AuctionWidget::maskLabel(){
	_errorLabel->setVisible(false);
}

void AuctionWidget::changeLabel(){
    _client->askCurrentPrice();
    _currentPrice=_client->getCurrentPrice();
    _priceLabel->setText(tr("Current price: %1").arg(_currentPrice));

    _nbOfBidders=(_currentPrice-_turnPrice)/(_startingPrice/10);
    _biddersLabel->setText(tr("Bidders: %1").arg(_nbOfBidders));
    update();
}

void AuctionWidget::startTurn() {
	_listAuctionsWidget->hide();
	_listAuctionsWidget->pause();

	if (_currentTurn==1) {
		_client->askAuctionTimeLeft(_auctionID);
   		_timeBeforeEndOfTurn=_client->getAuctionTimeLeft();
	}
	else _timeBeforeEndOfTurn=30;
    _client->askCurrentPrice();
    _currentPrice=_client->getCurrentPrice();
    _startingPrice = _listAuctionsWidget->getAuctionStartingPrice();
   	if (_currentTurn==1) _turnPrice=_startingPrice;
   	else _turnPrice = _currentPrice;

   	_updater->start();
	changeLabel();
	_priceLabel->setVisible(true);
	_biddersLabel->setVisible(true);
	_joinButton->setVisible(false);
	_joinButton->setEnabled(false);
	_bidButton->setEnabled(true);
	_bidButton->setVisible(true);
	_quitButton->setEnabled(true);
	_quitButton->setVisible(true);
   	_errorLabel->setText("");
  	_errorLabel->setVisible(true);
    _endOfTurnTimer->start();
}
