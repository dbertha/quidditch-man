#include "MainWindow.hpp"

MainWindow::MainWindow(int sockfd){
	_sockfd=sockfd;
	_client=new Client(sockfd);
	_stack = new QStackedWidget(this);
	_connexionPage = new ConnexionPage(_client,this);
	_stack->addWidget(_connexionPage);
    this->setCentralWidget(_stack);
    _stack->setCurrentIndex(0);

    setWindowTitle(tr("QUIDDITH MANAGER"));
    setFixedSize(800,640);

    setCentralWidget(_stack);

     QSignalMapper *mapper = new QSignalMapper(_stack);
	connect(mapper, SIGNAL(mapped(int)), _stack, SLOT(setCurrentIndex(int)));
 
	// Nav panel
	QWidget *panel = new QWidget(this);
	_stack->addWidget(panel); // index 0
	 
	QGridLayout *grid = new QGridLayout(this);
	panel->setLayout(grid);
	/*
	for (int r = 0; r < 3; ++r) {
		for (int c = 0; c < 3; ++c) {
			// Create the navigation button
			const int index = r * 3 + c + 1;
			QPushButton *navButton = new QPushButton(QString("%1").arg(index), panel);
			grid->addWidget(navButton, r, c, Qt::AlignCenter );
		 
			// and the matching 'application'
			QPushButton *button = new QPushButton(QString("App %1, click to end").arg(index), _stack);
			_stack->addWidget(button);
			connect(button, SIGNAL(clicked()), SLOT(goHome())); // needs some way to go back
			 
			// make the nav button switch
			mapper->setMapping(navButton, index);
			//connect(navButton, SIGNAL(clicked()), mapper, SLOT(map()));
		}
	}
	*/
}

void MainWindow::displayWindow(QWidget* window){
	QWidget* current = _stack->currentWidget();
	_stack->removeWidget(current);
	//_stack->addWidget(window);
	_stack->setCurrentWidget(window);
	delete current;
}
void MainWindow::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void MainWindow::quit(){
	close();
}

void MainWindow::cancel(){
	QWidget* current = _stack->currentWidget();
	_stack->removeWidget(current);
	delete current;
	_stack->setCurrentWidget(_connexionPage);
}

void MainWindow::enterLogin(bool registration){
	LoginPage* loginPage = new LoginPage(_client,this,registration);
	_stack->addWidget(loginPage);
	_stack->setCurrentWidget(loginPage);
}

void MainWindow::connexion(int role){
	MainGui* mainGUI = new MainGui(_client,_sockfd,this);
	mainGUI->show();
	mainGUI->role=role;
	mainGUI->createMenu();
	close();
}