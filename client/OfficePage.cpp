#include "OfficePage.hpp"

OfficePage::OfficePage(Client* client, MainWindow* parent) : _client(client),_parent(parent){
	setStyleSheet("OfficePage{background-image: url(Pictures/mainMenu.jpg)}");
	_players = new QPushButton(tr(" Players"));
	_auctions = new QPushButton(tr(" Auctions"));
	_buyAP = new QPushButton(tr("  Buy action points"));
	_gainAP = new QPushButton(tr("  Promote team"));
	_returnButton = new QPushButton(tr("     Return"));
	/*
	 QFont font("Elegansdsdn");
	font.setItalic(true);
	font.setPointSize(18);*/
	_players->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 20px \"Elegant Thin\", sans-serif; color: #8f6d25;text-align: left; }");
	_auctions->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 20px \"Elegant Thin\", sans-serif;color: #8f6d25;text-align: left; }");
	_buyAP->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 20px \"Elegant Thin\", sans-serif;color: #8f6d25;text-align: left; }");
	_gainAP->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 20px \"Elegant Thin\", sans-serif;color: #8f6d25;text-align: left; }");
	_returnButton->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 20px \"Elegant Thin\", sans-serif;color: #8f6d25;text-align: left; }");
	
	_players->setFlat(true);
	_auctions->setFlat(true);
	_buyAP->setFlat(true);
	_gainAP->setFlat(true);
	_returnButton->setFlat(true);


	_players->setCursor(Qt::PointingHandCursor);
	_auctions->setCursor(Qt::PointingHandCursor);
	_buyAP->setCursor(Qt::PointingHandCursor);
	_gainAP->setCursor(Qt::PointingHandCursor);
	_returnButton->setCursor(Qt::PointingHandCursor);
/*

	_stadium->setFont(font);
	_managerOffice->setFont(font);
	_domain->setFont(font);
*/
	_players->setMinimumWidth(200);
    _players->setMinimumHeight(35);
    _players->setMaximumWidth(200);
    _players->setMaximumHeight(35);
    _auctions->setMinimumWidth(200);
    _auctions->setMinimumHeight(35);
    _auctions->setMaximumWidth(200);
    _auctions->setMaximumHeight(35);
    _buyAP->setMinimumWidth(200);
    _buyAP->setMinimumHeight(35);
    _buyAP->setMaximumWidth(200);
    _buyAP->setMaximumHeight(35);
    _gainAP->setMinimumWidth(200);
    _gainAP->setMinimumHeight(35);
    _gainAP->setMaximumWidth(200);
    _gainAP->setMaximumHeight(35);
    _returnButton->setMinimumWidth(200);
    _returnButton->setMinimumHeight(35);
    _returnButton->setMaximumWidth(200);
    _returnButton->setMaximumHeight(35);


    QWidget *panel = new QWidget(this);
	 
	QGridLayout *grid = new QGridLayout(this);
	panel->setLayout(grid);
	
	grid->addWidget(_players, 2,2, Qt::AlignLeft );
	grid->addWidget(_auctions, 3,2, Qt::AlignLeft );
	grid->addWidget(_buyAP, 4,2, Qt::AlignLeft );
	grid->addWidget(_gainAP, 5,2, Qt::AlignLeft );
	grid->addWidget(_returnButton,9,2,Qt::AlignLeft );

	//_infos = new InfosWidget(_client,this);
	//grid->addWidget(_infos,0,0,1,2,Qt::AlignLeft);
	//580-780
	//220
	grid->setVerticalSpacing(0);
	grid->setHorizontalSpacing(0);
			//connect(navButton, SIGNAL(clicked()), mapper, SLOT(map()));

	grid->setColumnMinimumWidth(0,50);
	grid->setColumnMinimumWidth(1,520);
	grid->setColumnMinimumWidth(2,120);
	grid->setRowMinimumHeight(0,100);
	grid->setRowMinimumHeight(1,90);
	for (int i=2;i<=12;++i) {
		grid->setRowMinimumHeight(i,30);
	}


	_stack = new QStackedWidget(this);
	grid->addWidget(_stack,1,1,14,1,Qt::AlignCenter);
	_buyAPWidget = new BuyAPWidget(_client,this);
	_gainAPWidget = new GainAPWidget(_client,this);
	_managePlayerWidget = new ManagePlayerWidget(_client, this);
	_managePlayerWidget->pause();
	_auctionWidget = new AuctionWidget(_client,this);
	_auctionWidget->pause();
	_stack->addWidget(_buyAPWidget);
	_stack->addWidget(_gainAPWidget);
	_stack->addWidget(_managePlayerWidget);
	_stack->addWidget(_auctionWidget);
	
	_stack->setVisible(false);
/*
    QVBoxLayout *_mainLayout = new QVBoxLayout;
    _mainLayout->addWidget(_registerButton);
    _mainLayout->addWidget(_loginButton);
    _mainLayout->addWidget(_closeButton);
    setLayout(_mainLayout);
*/
    setWindowTitle(tr("QUIDDITH MANAGER"));
    setFixedSize(800,640);

    connect(_players,SIGNAL(clicked()),this,SLOT(listPlayers()));
    connect(_auctions,SIGNAL(clicked()),this,SLOT(listAuctions()));
    connect(_buyAP,SIGNAL(clicked()),this,SLOT(buyAP()));
    connect(_gainAP,SIGNAL(clicked()),this,SLOT(gainAP()));
    connect(_returnButton,SIGNAL(clicked()),this,SLOT(returnMenu()));
}

void OfficePage::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void OfficePage::pause() {
	//_infos->pause();
	_managePlayerWidget->pause();
	_auctionWidget->pause();}

void OfficePage::hide() {
	_stack->setVisible(false);
}


void OfficePage::resume() {
	//_infos->resume();
	_managePlayerWidget->resume();
	_auctionWidget->resume();}

void OfficePage::hideStack() {
	_buyAPWidget->maskLabel();
	_gainAPWidget->maskLabel();
	_stack->setVisible(false);
}

void OfficePage::showStack(){
	_stack->setVisible(true);
}
void OfficePage::update() {/*_infos->updateLabels();*/}

void OfficePage::blockButtons(){
	_players->setEnabled(false);
	_auctions->setEnabled(false);
	_buyAP->setEnabled(false);
	_gainAP->setEnabled(false);
	_returnButton->setEnabled(false);
}

void OfficePage::deblockButtons(){
	_players->setEnabled(true);
	_auctions->setEnabled(true);
	_buyAP->setEnabled(true);
	_gainAP->setEnabled(true);
	_returnButton->setEnabled(true);

}

void OfficePage::listPlayers(){
	_auctionWidget->pause();
	_managePlayerWidget->updateLabels();
	_managePlayerWidget->resume();
	_stack->setCurrentWidget(_managePlayerWidget);
	_stack->setVisible(true);
	_gainAPWidget->maskLabel();
	_buyAPWidget->maskLabel();
}

void OfficePage::listAuctions(){
	_managePlayerWidget->pause();
	_auctionWidget->updateLabels();
	_auctionWidget->resume();
	_stack->setCurrentWidget(_auctionWidget);
	_stack->setVisible(true);
	_gainAPWidget->maskLabel();
	_buyAPWidget->maskLabel();
	_managePlayerWidget->maskLabel();

}

void OfficePage::buyAP(){
	_stack->setCurrentWidget(_buyAPWidget);
	_stack->setVisible(true);
	_gainAPWidget->maskLabel();

}

void OfficePage::gainAP(){
	_stack->setCurrentWidget(_gainAPWidget);
	_buyAPWidget->maskLabel();
	_stack->setVisible(true);
}

void OfficePage::returnMenu(){
	//pause();

	_managePlayerWidget->maskLabel();
	_gainAPWidget->maskLabel();
	_buyAPWidget->maskLabel();
	_parent->mainPage();
	_stack->setVisible(false);
}