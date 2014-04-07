#include "MainWindow.hpp"
#include "MatchWindow.hpp"

MainWindow::MainWindow(int sockfd){
	_sockfd=sockfd;
	_client=new Client(sockfd);
	_stack = new QStackedWidget(this);
	_connexionPage = new ConnexionPage(_client,this);
	_stack->addWidget(_connexionPage);
    //this->setCentralWidget(_stack);
    _stack->setCurrentIndex(0);

    __pushesNotifier = new QSocketNotifier(_sockfd, QSocketNotifier::Read, this);
    __pushesNotifier->setEnabled(false); //genère le signal
    connect(__pushesNotifier,SIGNAL(activated(int)),this,SLOT(pushesHandler()));
    setWindowTitle(tr("QUIDDITH MANAGER"));
    //setFixedSize(780,620);

    //setCentralWidget(_stack);

     QSignalMapper *mapper = new QSignalMapper(_stack);
	connect(mapper, SIGNAL(mapped(int)), _stack, SLOT(setCurrentIndex(int)));
 
	// Nav panel
	QWidget *panel = new QWidget(this);
    //panel->setFixedSize(800,640);
	//_stack->addWidget(panel); // index 0
	grid = new QGridLayout(this);
    grid->setOriginCorner(Qt::TopLeftCorner);
    panel->setFixedSize(800,640);
	panel->setLayout(grid);


    //setCentralWidget(panel);
    //QVBoxLayout * mainLayout = new QVBoxLayout(this);
    //QWidget* test = new QWidget();
     //   test->setLayout(grid);
         
         
       // setCentralWidget(test);
    //setCentralWidget(panel);
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


    grid->setRowMinimumHeight(0,100);
    grid->setRowMinimumHeight(1,390);
    grid->setRowMinimumHeight(2,150);
    grid->setColumnMinimumWidth(0,50);
    grid->setColumnMinimumWidth(1,750);

    grid->addWidget(_stack,0,0,3,2,Qt::AlignLeft);

}

void MainWindow::displayWindow(QWidget* window){
	_stack->setCurrentWidget(window);
	
}
void MainWindow::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void MainWindow::quit(){
	reject();
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
	/*
	MainGui* mainGUI = new MainGui(_client,_sockfd,this);
	mainGUI->show();
	mainGUI->role=role;
	mainGUI->createMenu();
	close();
	*/
    if (role==NORMAL_LOGIN){

        _infos = new InfosWidget(_client,this);
        _select = new SelectPlayersWidget(_client,this);
        _notification = new NotificationWidget(_client,this,_select);
        grid->addWidget(_infos,0,0,1,2,Qt::AlignLeft);
        grid->addWidget(_notification,2,0,1,2,Qt::AlignLeft);
        grid->addWidget(_select,1,1,Qt::AlignLeft);
    	_mainPage = new MainPage(_client,this);
    	_stadiumPage= new StadiumPage(_client,this);
    	_officePage = new OfficePage(_client,this);
    	_domainPage = new DomainPage(_client,this);
    	_stack->addWidget(_mainPage);
    	_stack->addWidget(_stadiumPage);
    	_stack->addWidget(_officePage);
    	_stack->addWidget(_domainPage);
    	_stack->setCurrentWidget(_mainPage);
        
        _timerPause = new QTimer();
        _timerPause->setInterval(2000);
        _timerResume = new QTimer();
        _timerResume->setInterval(2000);
        connect(_timerPause,SIGNAL(timeout()),this,SLOT(pause()));
        connect(_timerResume,SIGNAL(timeout()),this,SLOT(resume()));
        _timerPause->start();
    }
    if (role==ADMIN_LOGIN){
        _adminPage = new AdminPage(_client,this);
        _stack->addWidget(_adminPage);
        _stack->setCurrentWidget(_adminPage);
    }
    
}

void MainWindow::mainPage(){
	_mainPage->update();
	_stack->setCurrentWidget(_mainPage);
	_mainPage->resume();
    _infos->setVisible(true);
}

void MainWindow::stadiumPage(){
	_stadiumPage->resume();
	_stack->setCurrentWidget(_stadiumPage);
    _infos->setVisible(true);
}

void MainWindow::officePage(){
	_domainPage->update();
	_stack->setCurrentWidget(_officePage);
	_domainPage->resume();
	_domainPage->hideStack();
    _infos->setVisible(true);
}

void MainWindow::domainPage(){
	_officePage->update();
	_stack->setCurrentWidget(_domainPage);
	_officePage->resume();
	_officePage->hideStack();
    _infos->setVisible(true);
}

void MainWindow::selectionPage(){
    _officePage->update();
    _officePage->resume();
    _officePage->hideStack();
    _infos->setVisible(true);
    _domainPage->update();
    _domainPage->resume();
    _domainPage->hideStack();
    _stack->setCurrentWidget(_select);
}

void MainWindow::pushesHandler(){
   
    __pushesNotifier->setEnabled(false);
    //receive
    SerializedObject received = receiveOnSocket(_sockfd);
    
    //handle
    switch(received.typeOfInfos){
		case MATCH_INVITATION : {
            int IDInvitor;
            char name[USERNAME_LENGTH];
            char * position = received.stringData;
            
            QString texte;
            std::vector<int> playersInTeam;
            memcpy(&IDInvitor, position, sizeof(IDInvitor));
            position += sizeof(IDInvitor);
            memcpy(&name, position, sizeof(name));

            _notification->friendlyMatchNotification(name,IDInvitor);

            break;
        }
        case SERVER_DOWN : {
            badConnection();
            break;
        }
        case MATCH_TOURNAMENT_START : {
            pause();
            block();
            int IDOpponent;
            char name[USERNAME_LENGTH];
            char * position = received.stringData;
            QString texte;
            //~ cout << "A tournament turn starts now !" << endl;
            memcpy(&IDOpponent, position, sizeof(IDOpponent));
            position += sizeof(IDOpponent);
            memcpy(&name, position, sizeof(name));
            position += sizeof(name);

            _notification->tournamentNotification(name,IDOpponent);
            //forced to accept

            break;
        }
	}
    __pushesNotifier->setEnabled(true);
}


void MainWindow::pause(){
    __pushesNotifier->setEnabled(false);
    _infos->pause();
    _mainPage->pause();
    _domainPage->pause();
    _stadiumPage->pause();
    _officePage->pause();
    _timerPause->stop();
    _timerResume->start();
    _select->pause();
}

void MainWindow::resume(){
    __pushesNotifier->setEnabled(true);
    _infos->resume();
    _mainPage->resume();
    _domainPage->resume();
    _stadiumPage->resume();
    _officePage->resume();
    _timerResume->stop();
    _timerPause->start();
    _select->resume();
}
void MainWindow::block(){
    _mainPage->blockButtons();
    _domainPage->blockButtons();
    _officePage->blockButtons();
    _stadiumPage->blockButtons();
}

void MainWindow::deblock(){
    _mainPage->deblockButtons();
    _domainPage->deblockButtons();
    _officePage->deblockButtons();
    _stadiumPage->deblockButtons();
}

void MainWindow::friendlyMatch() {
    _select->init(true,0);
}


void MainWindow::trainingMatch() {
    _select->init(false,-1);
}

int MainWindow::badConnection() {
    pause();
    _notification->noConnectionNotification();
    return (0);
}
void MainWindow::trainingMatchImpossibleNotification(){_notification->trainingMatchImpossibleNotification();}
void MainWindow::friendlyMatchDeniedNotification(){_notification->friendlyMatchDeniedNotification();}

void MainWindow::hideSelect(){
    _select->setVisible(false);
}

void MainWindow::hide(){
    _stadiumPage->hide();
    _officePage->hide();
    _domainPage->hide();

}