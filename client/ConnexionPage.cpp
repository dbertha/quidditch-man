#include "ConnexionPage.hpp"

#include <QtGui>

ConnexionPage::ConnexionPage(Client *client, MainWindow* parent) : _client(client),_parent(parent) {
	setStyleSheet("background-image: url(Pictures/connexionPage.jpg)");
	_loginButton = new QPushButton(tr("LOAD GAME"));
    _loginButton->setDefault(true);
    _loginButton->setMinimumWidth(200);
    _loginButton->setMinimumHeight(30);
    _loginButton->setMaximumWidth(200);
    _loginButton->setMaximumHeight(30);
    _loginButton->setCursor(Qt::PointingHandCursor);
    _registerButton = new QPushButton(tr("NEW GAME"));
    _registerButton->setDefault(true);
    _registerButton->setMinimumWidth(200);
    _registerButton->setMinimumHeight(30);
    _registerButton->setMaximumWidth(200);
    _registerButton->setMaximumHeight(30);
    _registerButton->setCursor(Qt::PointingHandCursor);
    _closeButton = new QPushButton(tr("QUIT"));
    _closeButton->setMinimumWidth(200);
    _closeButton->setMinimumHeight(30);
    _closeButton->setMaximumWidth(200);
    _closeButton->setMaximumHeight(30);
    _closeButton->setDefault(true);
    _closeButton->setCursor(Qt::PointingHandCursor);
    QColor col(209,173,77);
    if(col.isValid()) {
        QString qss = QString("background-color: %1;color: rgb(255, 255, 255)").arg(col.name());
        _loginButton->setStyleSheet(qss);
        _registerButton->setStyleSheet(qss);
        _closeButton->setStyleSheet(qss);
    }

	QWidget *panel = new QWidget(this);
	 
	QGridLayout *grid = new QGridLayout(this);
	panel->setLayout(grid);
	
	grid->addWidget(_loginButton, 9,2, Qt::AlignCenter );
	grid->addWidget(_registerButton, 10,2, Qt::AlignCenter );
	grid->addWidget(_closeButton, 11,2, Qt::AlignCenter );

	grid->setVerticalSpacing(3);
	grid->setHorizontalSpacing(0);
			//connect(navButton, SIGNAL(clicked()), mapper, SLOT(map()));

	for (int i=0;i<5;++i){
		grid->setColumnMinimumWidth(i,150);
		if (i==2) grid->setColumnMinimumWidth(i,200);
	}
	for (int i=0;i<19;++i){
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


    connect(_closeButton,SIGNAL(clicked()),this,SLOT(quit()));
    connect(_loginButton,SIGNAL(clicked()),this,SLOT(login()));
    connect(_registerButton,SIGNAL(clicked()),this,SLOT(newGame()));
    //exec();
}

void ConnexionPage::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void ConnexionPage::quit(){
	_parent->quit();
}

void ConnexionPage::login(){
	_parent->enterLogin();
}

void ConnexionPage::newGame(){
	_parent->enterLogin(true);
}