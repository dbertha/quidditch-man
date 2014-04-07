#include "AdminPage.hpp"
#include "Tournaments.hpp"

AdminPage::AdminPage(Client* client, MainWindow* parent) : _client(client),_parent(parent){
	setStyleSheet("AdminPage{background-image: url(Pictures/mainMenu.jpg)}");
	_listTournaments = new QPushButton(tr(" Tournaments"));
	_createTournament = new QPushButton(tr(" Create tournament"));
	_quitButton = new QPushButton(tr("     Quit"));
	/*
	 QFont font("Elegansdsdn");
	font.setItalic(true);
	font.setPointSize(18);*/
	_listTournaments->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 20px \"Elegant Thin\", sans-serif; color: #8f6d25;text-align: left; }");
	_createTournament->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 20px \"Elegant Thin\", sans-serif;color: #8f6d25;text-align: left; }");
	_quitButton->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 20px \"Elegant Thin\", sans-serif;color: #8f6d25;text-align: left; }");
	
	_listTournaments->setFlat(true);
	_createTournament->setFlat(true);
	_quitButton->setFlat(true);

	_createTournament->setEnabled(false);

	_listTournaments->setCursor(Qt::PointingHandCursor);
	_createTournament->setCursor(Qt::PointingHandCursor);
	_quitButton->setCursor(Qt::PointingHandCursor);
/*

	_stadium->setFont(font);
	_managerOffice->setFont(font);
	_domain->setFont(font);
*/
	_listTournaments->setMinimumWidth(200);
    _listTournaments->setMinimumHeight(35);
    _listTournaments->setMaximumWidth(200);
    _listTournaments->setMaximumHeight(35);
    _createTournament->setMinimumWidth(200);
    _createTournament->setMinimumHeight(35);
    _createTournament->setMaximumWidth(200);
    _createTournament->setMaximumHeight(35);
    _quitButton->setMinimumWidth(200);
    _quitButton->setMinimumHeight(35);
    _quitButton->setMaximumWidth(200);
    _quitButton->setMaximumHeight(35);

    _adminLabel = new QLabel(tr("ADMIN SESSION"));
    _adminLabel->setStyleSheet("QLabel{font: 28px \"Elegant Thin\", sans-serif;color: #c5c295;}");
    QWidget *panel = new QWidget(this);
	 
	_grid = new QGridLayout(this);
	panel->setLayout(_grid);

	_grid->addWidget(_adminLabel,0,1,Qt::AlignLeft);
	
	_grid->addWidget(_listTournaments, 2,2, Qt::AlignLeft );
	_grid->addWidget(_createTournament, 3,2, Qt::AlignLeft );
	_grid->addWidget(_quitButton,9,2,Qt::AlignLeft );

	//580-780
	//220
	_grid->setVerticalSpacing(0);
	_grid->setHorizontalSpacing(0);
			//connect(navButton, SIGNAL(clicked()), mapper, SLOT(map()));

	_grid->setColumnMinimumWidth(0,30);
	_grid->setColumnMinimumWidth(1,540);
	_grid->setColumnMinimumWidth(2,120);
	_grid->setRowMinimumHeight(0,100);
	_grid->setRowMinimumHeight(1,90);
	for (int i=2;i<=12;++i) {
		_grid->setRowMinimumHeight(i,30);
	}
	_stack = new QStackedWidget(this);
	_grid->addWidget(_stack,2,1,7,1);

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

    connect(_listTournaments,SIGNAL(clicked()),this,SLOT(listTournaments()));
    connect(_createTournament,SIGNAL(clicked()),this,SLOT(createTournament()));
    connect(_quitButton,SIGNAL(clicked()),this,SLOT(quit()));
}

void AdminPage::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}


void AdminPage::listTournaments(){
	
	chooseTournament(_client,ADMIN_LOGIN,this);
}

void AdminPage::createTournament(){
	
}


void AdminPage::quit() {_parent->quit();}