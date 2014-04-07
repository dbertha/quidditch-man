#include "StadiumPage.hpp"
#include "Tournaments.hpp"
#include "MatchWindow.hpp"
#include "ClientMatchHandler.hpp"

StadiumPage::StadiumPage(Client* client, MainWindow* parent) : _client(client),_parent(parent){
	setStyleSheet("StadiumPage{background-image: url(Pictures/mainMenu.jpg)}");
	_friendlyMatch = new QPushButton(tr(" Friendly match"));
	_tournaments = new QPushButton(tr(" Tournaments"));
	_trainingMatch = new QPushButton(tr("  Training match"));
	_returnButton = new QPushButton(tr("     Return"));
	/*
	 QFont font("Elegansdsdn");
	font.setItalic(true);
	font.setPointSize(18);*/
	_friendlyMatch->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 20px \"Elegant Thin\", sans-serif; color: #8f6d25;text-align: left; }");
	_tournaments->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 20px \"Elegant Thin\", sans-serif;color: #8f6d25;text-align: left; }");
	_trainingMatch->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 20px \"Elegant Thin\", sans-serif;color: #8f6d25;text-align: left; }");
	_returnButton->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 20px \"Elegant Thin\", sans-serif;color: #8f6d25;text-align: left; }");
	
	_friendlyMatch->setFlat(true);
	_tournaments->setFlat(true);
	_trainingMatch->setFlat(true);
	_returnButton->setFlat(true);


	_friendlyMatch->setCursor(Qt::PointingHandCursor);
	_tournaments->setCursor(Qt::PointingHandCursor);
	_trainingMatch->setCursor(Qt::PointingHandCursor);
	_returnButton->setCursor(Qt::PointingHandCursor);
/*

	_stadium->setFont(font);
	_managerOffice->setFont(font);
	_domain->setFont(font);
*/
	_friendlyMatch->setMinimumWidth(200);
    _friendlyMatch->setMinimumHeight(35);
    _friendlyMatch->setMaximumWidth(200);
    _friendlyMatch->setMaximumHeight(35);
    _tournaments->setMinimumWidth(200);
    _tournaments->setMinimumHeight(35);
    _tournaments->setMaximumWidth(200);
    _tournaments->setMaximumHeight(35);
    _trainingMatch->setMinimumWidth(200);
    _trainingMatch->setMinimumHeight(35);
    _trainingMatch->setMaximumWidth(200);
    _trainingMatch->setMaximumHeight(35);
    _returnButton->setMinimumWidth(200);
    _returnButton->setMinimumHeight(35);
    _returnButton->setMaximumWidth(200);
    _returnButton->setMaximumHeight(35);


    QWidget *panel = new QWidget(this);
	 
	QGridLayout *grid = new QGridLayout(this);
	panel->setLayout(grid);
	
	grid->addWidget(_friendlyMatch, 2,1, Qt::AlignLeft );
	grid->addWidget(_tournaments, 3,1, Qt::AlignLeft );
	grid->addWidget(_trainingMatch, 4,1, Qt::AlignLeft );
	grid->addWidget(_returnButton,9,1,Qt::AlignLeft );

	//_infos = new InfosWidget(_client,this);
	//grid->addWidget(_infos,0,0,Qt::AlignLeft);
	//580-780
	//220
	grid->setVerticalSpacing(3);
	grid->setHorizontalSpacing(0);
			//connect(navButton, SIGNAL(clicked()), mapper, SLOT(map()));

	grid->setColumnMinimumWidth(0,570);
	grid->setColumnMinimumWidth(1,120);
	grid->setRowMinimumHeight(0,100);
	grid->setRowMinimumHeight(1,90);
	for (int i=2;i<=12;++i) {
		grid->setRowMinimumHeight(i,30);
	}


    _stack = new QStackedWidget();
    _tournamentsWidget = new TournamentsWidget(_client,this,NORMAL_LOGIN);
    _stack->addWidget(_tournamentsWidget);
    _stack->setVisible(false);
    grid->addWidget(_stack,2,0,10,1,Qt::AlignCenter);
/*
    QVBoxLayout *_mainLayout = new QVBoxLayout;
    _mainLayout->addWidget(_registerButton);
    _mainLayout->addWidget(_loginButton);
    _mainLayout->addWidget(_closeButton);
    setLayout(_mainLayout);
*/
    setWindowTitle(tr("QUIDDITH MANAGER"));
    setFixedSize(800,640);

    connect(_friendlyMatch,SIGNAL(clicked()),this,SLOT(friendlyMatch()));
    connect(_tournaments,SIGNAL(clicked()),this,SLOT(tournaments()));
    connect(_trainingMatch,SIGNAL(clicked()),this,SLOT(trainingMatch()));
    connect(_returnButton,SIGNAL(clicked()),this,SLOT(returnMenu()));
}

void StadiumPage::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void StadiumPage::pause() {
    //_infos->pause();
    _tournamentsWidget->pause();
}
void StadiumPage::resume() {
    //_infos->resume();
    _tournamentsWidget->resume();
}

void StadiumPage::friendlyMatch(){
    _stack->setVisible(false);
    _parent->friendlyMatch();
}

void StadiumPage::tournaments(){
    _stack->setCurrentWidget(_tournamentsWidget);
    _stack->setVisible(true);
}

void StadiumPage::trainingMatch(){
    _stack->setVisible(false);
    _parent->trainingMatch();
}

void StadiumPage::returnMenu(){
	_stack->setVisible(false);
	_parent->mainPage();
}

void StadiumPage::blockButtons(){
	_friendlyMatch->setEnabled(false);
	_tournaments->setEnabled(false);
	_trainingMatch->setEnabled(false);
	_returnButton->setEnabled(false);
}

void StadiumPage::deblockButtons(){
	_friendlyMatch->setEnabled(true);
	_tournaments->setEnabled(true);
	_trainingMatch->setEnabled(true);
	_returnButton->setEnabled(true);
}