#include "ListPlayersWidget.hpp"

ListPlayersWidget::ListPlayersWidget(Client* client, QWidget* parent) : _client(client),_parent(parent){
	
	setStyleSheet("ListPlayersWidget{background-image: url(Pictures/transparent.png);}");
	_noPlayerLabel = new QLabel("There isn't any player");
	_noPlayerLabel->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:red;}");
	_noPlayerLabel->setAlignment(Qt::AlignCenter);
	

	_listPlayers = new QListWidget();
	_listPlayers->setMinimumHeight(40);
	_listPlayers->setMaximumHeight(40);

	
	std::vector<std::string> names = _client->receivePlayersList();

	_stack = new QStackedWidget();

	_stack->addWidget(_noPlayerLabel);

	for (unsigned i=0;i<names.size();i+=2){
		std::string tmp=names[i]+" "+names[i+1];
		_playersName.push_back(tmp);
		QString name= QString(tr(tmp.c_str()));
		_listPlayers->addItem(name);
		_players.push_back(new PlayerWidget(_client,this,name,i/2,false));
		_stack->addWidget(_players[i/2]);
	}
	if (_listPlayers->count()!=0){
		_noPlayerLabel->setText(tr("Select a player"));
	
		_listPlayers->setCurrentRow(0);
		_stack->setCurrentWidget(_players[0]);
	}
	else{
		_stack->setCurrentWidget(_noPlayerLabel);
	}

	
    QWidget *panel = new QWidget(this);
	 
	QGridLayout *grid = new QGridLayout(this);
	panel->setLayout(grid);
	

	grid->setRowMinimumHeight(0,30);
	grid->setRowMinimumHeight(1,270);
	grid->setColumnMinimumWidth(0,500);

	grid->setVerticalSpacing(0);
	grid->setHorizontalSpacing(0);

	grid->addWidget(_listPlayers,0,0,Qt::AlignCenter);
	grid->addWidget(_stack,1,0,Qt::AlignCenter);

	_timer = new QTimer();
	connect(_timer, SIGNAL(timeout()),this,SLOT(update()));
	_timer->setInterval(1000);
	_timer->start();
	setFixedSize(500,340);

	connect(_listPlayers,SIGNAL(itemSelectionChanged()),this,SLOT(displayPlayer()));
}

void ListPlayersWidget::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void ListPlayersWidget::pause() {_timer->stop();}
void ListPlayersWidget::resume() {_timer->start();}

void ListPlayersWidget::displayPlayer(){
	//dynamic_cast<ManagePlayerWidget*>(_parent)->maskLabel();
	if (_listPlayers->currentRow()>=0) _stack->setCurrentWidget(_players[_listPlayers->currentRow()]);
	else _stack->setCurrentWidget(_noPlayerLabel);
}

void ListPlayersWidget::update(){
	std::vector<std::string> names =  _client->receivePlayersList();
	std::vector<std::string> tmpList;
	//std::vector<int> auctionIDs;
	for (int i=0;i<names.size();i+=2){
		std::string tmp=names[i]+" "+names[i+1];
		tmpList.push_back(tmp);
		//auctionIDs.push_back(tr(names[i].c_str()).toInt());
	}
	bool alreadyHere,stillHere;
	for (unsigned i=0;i<names.size();i+=2){
		alreadyHere=false;
		for (unsigned j=0;j<_playersName.size();++j){
			if (tmpList[i/2]==_playersName[j]){
				alreadyHere=true;
				_players[j]->setNewID(i/2);
				_players[j]->updateLabels();
			}
		}
		if (!alreadyHere){
			QString name= QString(tr(tmpList[i/2].c_str()));
			
			_players.push_back(new PlayerWidget(_client,this,name,i/2,false));
			_stack->addWidget(_players.back());
			_listPlayers->addItem(name);
		}
	}
	int nbRemoved=0;
	for (unsigned i=0;i<_playersName.size();++i){
		stillHere=false;
		for (unsigned j=0;j<tmpList.size();++j){
			if (_playersName[i]==tmpList[j]){
				stillHere=true;
			}
		}
		if (!stillHere){
			if (_listPlayers->count()==1){
				_stack->setCurrentWidget(_noPlayerLabel);
			}

			_listPlayers->takeItem(i-nbRemoved); 
			
			_stack->removeWidget(_players[i-nbRemoved]);
			delete _players[i-nbRemoved];
			_players.erase(_players.begin()+i-nbRemoved);
			
			++nbRemoved;

		}
	}
	_playersName=tmpList;
	if (_listPlayers->count()!=0){
		_noPlayerLabel->setText(tr("Select a player"));

	}
	else {

		_noPlayerLabel->setText(tr("There isn't any player available"));
	}
	
}

std::vector<std::string> ListPlayersWidget::getNamesList(){
	return _playersName;
}

int ListPlayersWidget::currentRow(){
	return _listPlayers->currentRow();
}

bool ListPlayersWidget::isPlayerBlocked(int row){
	std::vector<int> infos=_client->receivePlayerInfo(row);
	return infos[10];
}

int ListPlayersWidget::getCurrentPlayer(){return _listPlayers->currentRow();}

void ListPlayersWidget::hidePlayer(int row){
	_listPlayers->item(row)->setHidden(true);
	for (int i=0;i<_listPlayers->count();++i){
		if (!_listPlayers->item(i)->isHidden()) {
			_listPlayers->setCurrentRow(i);
			i=_listPlayers->count();
		}
	}
}

void ListPlayersWidget::showPlayers(){
	for (int i=0;i<_listPlayers->count();++i){
		_listPlayers->item(i)->setHidden(false);
	}
}
