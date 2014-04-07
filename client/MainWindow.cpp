#include "MainWindow.hpp"
#include "MatchWindow.hpp"
#include "ClientMatchHandler.hpp"
MainWindow::MainWindow(int sockfd){
	_sockfd=sockfd;
	_client=new Client(sockfd);
	_stack = new QStackedWidget(this);
	_connexionPage = new ConnexionPage(_client,this);
	_stack->addWidget(_connexionPage);
    this->setCentralWidget(_stack);
    _stack->setCurrentIndex(0);

    __pushesNotifier = new QSocketNotifier(_sockfd, QSocketNotifier::Read, this);
    __pushesNotifier->setEnabled(false); //genère le signal
    connect(__pushesNotifier,SIGNAL(activated(int)),this,SLOT(pushesHandler()));
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
	_stack->setCurrentWidget(window);
	
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
	/*
	MainGui* mainGUI = new MainGui(_client,_sockfd,this);
	mainGUI->show();
	mainGUI->role=role;
	mainGUI->createMenu();
	close();
	*/
    if (role==NORMAL_LOGIN){
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
}

void MainWindow::stadiumPage(){
	_stadiumPage->resume();
	_stack->setCurrentWidget(_stadiumPage);
}

void MainWindow::officePage(){
	_domainPage->update();
	_stack->setCurrentWidget(_officePage);
	_domainPage->resume();
	_domainPage->hideStack();
}

void MainWindow::domainPage(){
	_officePage->update();
	_stack->setCurrentWidget(_domainPage);
	_officePage->resume();
	_officePage->hideStack();
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
            bool confirmation;
            QString texte;
            std::vector<int> playersInTeam;
            memcpy(&IDInvitor, position, sizeof(IDInvitor));
            position += sizeof(IDInvitor);
            memcpy(&name, position, sizeof(name));
            //texte << "Do you want to fight against " << name << "with ID : " << QString::number(IDInvitor) << " ?";
            texte = QString("%1, with ID %2 wants to play a match against you !").arg(name, QString::number(IDInvitor));
            QMessageBox msgBox;
            msgBox.setWindowTitle("You've got a match proposal !");
            msgBox.setText(texte);
            msgBox.setInformativeText("Do you accept the match ?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            if(msgBox.exec() == QMessageBox::Yes){
                confirmation = true;
            }else {
                confirmation = false;
            }
            
            if(confirmation){
                playersInTeam = chooseTeamForMatch(_client, this);
            }
            _client->answerMatchProposal(confirmation, playersInTeam);
            //~ answerMatchProposal(confirmation, playersInTeam); //liste vide = refus de l'invitation
            if(_client->receiveMatchConfirmation() == MATCH_STARTING){
                MatchWindow * matchWindow  = new MatchWindow(_client, 2, this);
                matchWindow->show();
                //~ //startMatch( 2); //invité a l'équipe 2
            }
            break;
        }
        case SERVER_DOWN : {
            //badConnection();
            break;
        }
        case MATCH_TOURNAMENT_START : {
            int IDOpponent, numTeam;
            char name[USERNAME_LENGTH];
            char * position = received.stringData;
            QString texte;
            //~ cout << "A tournament turn starts now !" << endl;
            memcpy(&IDOpponent, position, sizeof(IDOpponent));
            position += sizeof(IDOpponent);
            memcpy(&name, position, sizeof(name));
            position += sizeof(name);
            texte = QString("%1, with ID %2 is your opponent !").arg(name, QString::number(IDOpponent));
            QMessageBox msgBox;
            msgBox.setWindowTitle("A tournament match starts now !");
            msgBox.setText(texte);
            msgBox.setInformativeText("You have to accept.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
            //forced to accept
            std::vector<int> playersInTeam = chooseTeamForMatch(_client, this);
            _client->sendTeamForMatchTournament(playersInTeam);
            //bloquant, l'adversaire doit avoir répondu aussi :
            numTeam = _client->receiveNumOfTeam();
            if(numTeam > 0){ //first to answer is the team 1
                MatchWindow * matchWindow = new MatchWindow(_client, numTeam, this);
                matchWindow->show();
                //~ startMatch(numTeam);
            }
            break;
        }
	}
    __pushesNotifier->setEnabled(true);
}

void MainWindow::pause(){
    __pushesNotifier->setEnabled(false);
    _mainPage->pause();
    _domainPage->pause();
    _stadiumPage->pause();
    _officePage->pause();
    _timerPause->stop();
    _timerResume->start();
}

void MainWindow::resume(){
    __pushesNotifier->setEnabled(true);
    _mainPage->resume();
    _domainPage->resume();
    _stadiumPage->resume();
    _officePage->resume();
    _timerResume->stop();
    _timerPause->start();
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