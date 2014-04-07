#include "MainPage.hpp"


MainPage::MainPage(Client* client, MainWindow* parent) : _client(client),_parent(parent){
	setStyleSheet("MainPage{background-image: url(Pictures/mainMenu.jpg)}");
	_stadium = new QPushButton(tr("  Stadium"));
	_managerOffice = new QPushButton(tr("  Manager office"));
	_domain = new QPushButton(tr("  Domain"));
	_quit = new QPushButton(tr("     Quit"));
	/*
	 QFont font("Elegansdsdn");
	font.setItalic(true);
	font.setPointSize(18);*/
	_stadium->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 22px \"Elegant Thin\", sans-serif; color: #8f6d25;text-align: left; }");
	_managerOffice->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 22px \"Elegant Thin\", sans-serif;color: #8f6d25;text-align: left; }");
	_domain->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 22px \"Elegant Thin\", sans-serif;color: #8f6d25;text-align: left; }");
	_quit->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 22px \"Elegant Thin\", sans-serif;color: #8f6d25;text-align: left; }");
	
	_stadium->setFlat(true);
	_managerOffice->setFlat(true);
	_domain->setFlat(true);
	_quit->setFlat(true);


	_stadium->setCursor(Qt::PointingHandCursor);
	_managerOffice->setCursor(Qt::PointingHandCursor);
	_domain->setCursor(Qt::PointingHandCursor);
	_quit->setCursor(Qt::PointingHandCursor);
/*
	_stadium->setFont(font);
	_managerOffice->setFont(font);
	_domain->setFont(font);
*/
	_stadium->setMinimumWidth(200);
    _stadium->setMinimumHeight(35);
    _stadium->setMaximumWidth(200);
    _stadium->setMaximumHeight(35);
    _managerOffice->setMinimumWidth(200);
    _managerOffice->setMinimumHeight(35);
    _managerOffice->setMaximumWidth(200);
    _managerOffice->setMaximumHeight(35);
    _domain->setMinimumWidth(200);
    _domain->setMinimumHeight(35);
    _domain->setMaximumWidth(200);
    _domain->setMaximumHeight(35);
    _quit->setMinimumWidth(200);
    _quit->setMinimumHeight(35);
    _quit->setMaximumWidth(200);
    _quit->setMaximumHeight(35);

    QWidget *panel = new QWidget(this);
	 
	QGridLayout *grid = new QGridLayout(this);
	panel->setLayout(grid);
	
	grid->addWidget(_stadium, 2,1, Qt::AlignLeft );
	grid->addWidget(_managerOffice, 3,1, Qt::AlignLeft );
	grid->addWidget(_domain, 4,1, Qt::AlignLeft );
	grid->addWidget(_quit,9,1,Qt::AlignLeft);

	_infos = new InfosWidget(_client,this);
	grid->addWidget(_infos,0,0,Qt::AlignLeft);
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


/*
    QVBoxLayout *_mainLayout = new QVBoxLayout;
    _mainLayout->addWidget(_registerButton);
    _mainLayout->addWidget(_loginButton);
    _mainLayout->addWidget(_closeButton);
    setLayout(_mainLayout);
*/
    setWindowTitle(tr("QUIDDITH MANAGER"));
    setFixedSize(800,640);

    connect(_stadium,SIGNAL(clicked()),this,SLOT(enterStadium()));
    connect(_managerOffice,SIGNAL(clicked()),this,SLOT(enterManagerOffice()));
    connect(_domain,SIGNAL(clicked()),this,SLOT(visitDomain()));
    connect(_quit,SIGNAL(clicked()),this,SLOT(quit()));
}

void MainPage::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void MainPage::pause() {_infos->pause();}
void MainPage::resume() {_infos->resume();}

void MainPage::enterStadium(){
	//pause();
	_parent->stadiumPage();
}

void MainPage::enterManagerOffice(){
	//pause();
	_parent->officePage();
}

void MainPage::visitDomain(){
	//pause();
	_parent->domainPage();
}

void MainPage::quit() {_parent->quit();}

void MainPage::blockButtons(){
	_stadium->setEnabled(false);
	_managerOffice->setEnabled(false);
	_domain->setEnabled(false);
	_quit->setEnabled(false);
}

void MainPage::deblockButtons(){
	_stadium->setEnabled(true);
	_managerOffice->setEnabled(true);
	_domain->setEnabled(true);
	_quit->setEnabled(true);
}