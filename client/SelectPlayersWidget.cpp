#include "SelectPlayersWidget.hpp"

SelectPlayersWidget::SelectPlayersWidget(Client* client, MainWindow* parent) : \
_client(client),_parent(parent){
	
	setVisible(false);
	setStyleSheet("SelectPlayersWidget{ background-color: rgba(255, 255, 255, 130);}");

	_listManagers = new QListWidget();
	_listManagers->setMinimumHeight(40);
	_listManagers->setMaximumHeight(40);

	_noManagerLabel = new QLabel("");
	_noManagerLabel->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:red;}");
	_noManagerLabel->setAlignment(Qt::AlignCenter);

	
    _client->receiveManagersIDandNames(&_IDList,&_namesList);
	_listPlayersWidget = new ListPlayersWidget(_client,this);


	_listManagers->setVisible(false);

	_friendlyMatch = new QPushButton(tr("Friendly match"));

	_rolesName.push_back(QString(tr("Keeper")));
	_rolesName.push_back(QString(tr("Seeker")));
	_rolesName.push_back(QString(tr("First chaser")));
	_rolesName.push_back(QString(tr("Second chaser")));
	_rolesName.push_back(QString(tr("Third chaser")));
	_rolesName.push_back(QString(tr("First Beater")));
	_rolesName.push_back(QString(tr("Second Beater")));


    QWidget *panel = new QWidget(this);
	 
	QGridLayout *grid = new QGridLayout(this);
	panel->setLayout(grid);
	

	for (int i=0;i<_rolesName.size();++i){
		_roles.push_back(new QPushButton(_rolesName[i]));
		grid->addWidget(_roles[i],2,1,Qt::AlignCenter);
		connect(_roles[i],SIGNAL(clicked()),this,SLOT(choosePlayer()));
	}

	_confirmButton = new QPushButton(tr("Confirm"));
	_cancelButton = new QPushButton(tr("Cancel"));

	hideButtons();

	if (!_ask) {
		_listManagers->setVisible(false);
		_noManagerLabel->setVisible(false);
		_listPlayersWidget->setVisible(true);
		_roles[0]->setVisible(true);
	}

	grid->setRowMinimumHeight(0,40);
	grid->setRowMinimumHeight(1,340);
	grid->setRowMinimumHeight(2,30);
	grid->setRowMinimumHeight(3,30);
	grid->setColumnMinimumWidth(0,250);
	grid->setColumnMinimumWidth(1,250);

	grid->setVerticalSpacing(0);
	grid->setHorizontalSpacing(0);

	grid->addWidget(_friendlyMatch,2,1,Qt::AlignCenter);
	grid->addWidget(_listManagers,0,0,1,2,Qt::AlignCenter);
	grid->addWidget(_listPlayersWidget,1,0,1,2,Qt::AlignCenter);
	grid->addWidget(_noManagerLabel,1,0,1,2,Qt::AlignCenter);
	grid->addWidget(_cancelButton,2,0,Qt::AlignCenter);
	grid->addWidget(_confirmButton,2,1,Qt::AlignCenter);

	setFixedSize(500,500);


	_timer = new QTimer();
	_timer->setInterval(1000);
	connect(_timer,SIGNAL(timeout()),this,SLOT(updateListManagers()));
	

	connect(_cancelButton,SIGNAL(clicked()),this,SLOT(cancel()));
	connect(_confirmButton,SIGNAL(clicked()),this,SLOT(confirm()));
	connect(_friendlyMatch,SIGNAL(clicked()),this,SLOT(friendlyMatch()));
	connect(_listManagers,SIGNAL(itemSelectionChanged()),this,SLOT(activateButton()));
}

void SelectPlayersWidget::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void SelectPlayersWidget::pause() {_listPlayersWidget->pause();}
void SelectPlayersWidget::resume() {_listPlayersWidget->resume();}


void SelectPlayersWidget::activateButton(){_friendlyMatch->setVisible(true);}

void SelectPlayersWidget::updateListManagers() {
	_client->receiveManagersIDandNames(&_IDList,&_namesList);

	int previousRow = _listManagers->currentRow();
	_listManagers->clear();
	for (unsigned i=0;i<_namesList.size();++i){
			
		QString name= QString(tr(_namesList[i].c_str()));
				
		_listManagers->addItem(name);
	}
	
	if (_listManagers->count()!=0){
		if (_listManagers->currentRow()<0) {
			_noManagerLabel->setText(tr("Select a manager"));
			hideButtons();
		
			}

	}
	else {

		_noManagerLabel->setText(tr("There isn't any manager connected"));
	}
	
	update();

}

void SelectPlayersWidget::friendlyMatch(){
	_listManagers->setVisible(false);
	_noManagerLabel->setVisible(false);
	_listPlayersWidget->pause();
	_timer->stop();
	_opponentID=_IDList[_listManagers->currentRow()];
	
	//_listPlayersWidget->pause();
	_listPlayersWidget->update();
	_listPlayersWidget->setVisible(true);
	_team.clear();
	_roles[0]->setVisible(true);
	_cancelButton->setVisible(true);
	_friendlyMatch->setVisible(false);
}

void SelectPlayersWidget::choosePlayer(){
	_listPlayersWidget->pause();
	int row= _listPlayersWidget->currentRow();
	if (row!=-1){
		_team.push_back(row);
		_roles[_team.size()-1]->setVisible(false);
		_listPlayersWidget->hidePlayer(row);
		if (_team.size()<7) _roles[_team.size()]->setVisible(true);
		else {
			_listPlayersWidget->setVisible(false);
			_confirmButton->setVisible(true);
		}
	}
}

void SelectPlayersWidget::maskLabel(){
	_errorLabel->setVisible(false);
}

void SelectPlayersWidget::init(bool value, int opponentID) {
	_ask=value;
	_opponentID=opponentID;
	_team.clear();
	setVisible(true);
	if (value&&_opponentID==0){//asks 
		_listManagers->setVisible(true);
		_listManagers->update();
		hideButtons();
		_noManagerLabel->setVisible(true);
		//_cancelButton->setVisible(true);
		_timer->start();
	}
	else{ //answers or tournament or training
		_listManagers->setVisible(false);
		_listPlayersWidget->showPlayers();
		_noManagerLabel->setVisible(false);
		_listPlayersWidget->setVisible(true);
		_roles[0]->setVisible(true);
		_cancelButton->setVisible(false);
		_timer->stop();
	}

}

void SelectPlayersWidget::hideButtons(){
	_listPlayersWidget->setVisible(false);
	for (int i=0;i<7;++i){
		_roles[i]->setVisible(false);
	}
	_friendlyMatch->setVisible(false);
	_confirmButton->setVisible(false);
	_cancelButton->setVisible(false);

}

void SelectPlayersWidget::cancel(){
	_team.clear();
	init(_ask,0);

}

void SelectPlayersWidget::confirm(){
	hideButtons();
	setVisible(false);
	if (!_ask&&_opponentID==-1){
        _parent->pause();
	    _parent->block();
	    //std::vector<int> chosenPlayers;
	    //chosenPlayers = chooseTeamForMatch(_client, this); //tous les rôles sont nécessairement remplis 
	    _client->sendTrainingMatchRequest(_team);
	    int confirmation = _client->receiveMatchConfirmation();
	    if(confirmation == 1){
	        MatchWindow * matchWindow = new MatchWindow(_client, 1, _parent);
	        matchWindow->show();
	    }else{
	       _parent->trainingMatchImpossibleNotification();
	    }
	    //_parent->deblock();
	    _parent->resume();
	}
	else if (!_ask&&_opponentID==0){

		//std::vector<int> playersInTeam = chooseTeamForMatch(_client, this);
		_client->sendTeamForMatchTournament(_team);
		//bloquant, l'adversaire doit avoir répondu aussi :
		int numTeam = _client->receiveNumOfTeam();
		if(numTeam > 0){ //first to answer is the team 1
		MatchWindow * matchWindow = new MatchWindow(_client, numTeam, _parent);
		matchWindow->show();
		//~ startMatch(numTeam);
	}
	else if (_ask&&_opponentID==-1)
		
		_client->answerMatchProposal(true, _team);
		//~ answerMatchProposal(confirmation, playersInTeam); //liste vide = refus de l'invitation
		if(_client->receiveMatchConfirmation() == MATCH_STARTING){
			MatchWindow * matchWindow  = new MatchWindow(_client, 2, _parent);
			matchWindow->show();
			//~ //startMatch( 2); //invité a l'équipe 2
		}
		
	}
	else {
		_parent->pause();
		_parent->block();
		_client->proposeMatchTo(_opponentID, _team);

		int confirmation = _client->receiveMatchConfirmation();
		//~ progress->setValue(1);
		if(confirmation == MATCH_STARTING){

			_parent->block();
			MatchWindow * matchWindow = new MatchWindow(_client, 1, _parent);
			matchWindow->show();
				    //startMatch(1); //inviteur a l'équipe 1
		}
		else{
			_parent->friendlyMatchDeniedNotification();
		}
		
		// __pushesNotifier->setEnabled(true);
		//_parent->deblock();
		_parent->resume();
	}
	setVisible(false);
}