#include "TournamentsWidget.hpp"

TournamentsWidget::TournamentsWidget(Client* client, QWidget* parent, int role) : _client(client),_parent(parent),_role(role){
	
	setStyleSheet("TournamentsWidget{ background-color: rgba(255, 255, 255, 100);}");
	
	_listTournaments = new QListWidget();
	_listTournaments->setMinimumHeight(50);
	_listTournaments->setMaximumHeight(50);
	_listTournaments->setMinimumWidth(150);

	_listTournaments->setVisible(false);
	
	_currentParticipants = new QLabel(tr("Participants : %1/%2").arg(0).arg(0));
	_startingPrice = new QLabel(tr("Starting price : %1 gold").arg(0));
	_currentParticipants->setVisible(false);
	_startingPrice->setVisible(false);
	_currentParticipants->setStyleSheet("QLabel{font: 18px \"Elegant Thin\", sans-serif;color: #53502d;}");
	_startingPrice->setStyleSheet("QLabel{font: 18px \"Elegant Thin\", sans-serif;color: #53502d;}");


 	QWidget *panel = new QWidget(this);
	 
	QGridLayout* grid = new QGridLayout(this);
	panel->setLayout(grid);

	grid->setRowMinimumHeight(0,50);
	grid->setRowMinimumHeight(1,30);
	grid->setRowMinimumHeight(2,30);
	grid->setRowMinimumHeight(3,30);
	grid->setRowMinimumHeight(4,30);
	grid->setRowMinimumHeight(5,30);
	grid->setColumnMinimumWidth(0,100);
	grid->setColumnMinimumWidth(1,200);
	grid->setColumnMinimumWidth(2,100);

	grid->setVerticalSpacing(0);
	grid->setHorizontalSpacing(0);

	grid->addWidget(_listTournaments,0,0,1,3,Qt::AlignCenter);
	grid->addWidget(_currentParticipants,1,0,1,3,Qt::AlignLeft);
	grid->addWidget(_startingPrice,2,0,1,3,Qt::AlignLeft);

	_label = new QLabel();
	grid->addWidget(_label,5,0,1,3,Qt::AlignCenter);
	_label->setVisible(false);

	if (role==ADMIN_LOGIN){
		_price = new QLineEdit();
		QIntValidator* validator = new QIntValidator(10000, 99999999, this);
		_price->setValidator(validator);
		_price->setMaximumHeight(30);
		_price->setMinimumWidth(200);
		_price->setMaximumWidth(200);
		_participants = new QLineEdit();
		QIntValidator* validator2 = new QIntValidator(2, 32, this);
		_participants->setValidator(validator2);
		_participants->setMaximumHeight(30);
		_participants->setMaximumWidth(100);
		_create=new QPushButton("Create");

		QLabel* price = new QLabel(tr("Starting price:"));
		QLabel* participants = new QLabel(tr("Participants:"));
		grid->addWidget(_participants,4,0,Qt::AlignLeft);
		grid->addWidget(_price,4,1,Qt::AlignLeft);
		grid->addWidget(_create,4,2,Qt::AlignLeft);
		grid->addWidget(price,3,1,Qt::AlignLeft);
		grid->addWidget(participants,3,0,Qt::AlignLeft);
		connect(_create,SIGNAL(clicked()),this,SLOT(create()));
	}
	else {
		_join=new QPushButton("Join");
		grid->addWidget(_join,4,0,1,3,Qt::AlignCenter);
		connect(_join,SIGNAL(clicked()),this,SLOT(join()));
	}
	_timer = new QTimer();
	_timer->setInterval(1000);

	setFixedSize(400,210);
	connect(_timer,SIGNAL(timeout()),this,SLOT(updateLabels()));
	
	connect(_listTournaments,SIGNAL(itemSelectionChanged()),this,SLOT(displayTournament()));
	_timer->start();
}

void TournamentsWidget::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void TournamentsWidget::pause() {_timer->stop();}
void TournamentsWidget::resume() {_timer->start();}

void TournamentsWidget::maskLabel(){
	_currentParticipants->setVisible(false);
	_startingPrice->setVisible(false);
	_label->setVisible(false);
}

void TournamentsWidget::displayTournament(){
	if (_listTournaments->count()>0 && _listTournaments->currentRow()>=0) {
		_currentParticipants->setVisible(true);
		_startingPrice->setVisible(true);
	}
	else {
		maskLabel();
	}
}

void TournamentsWidget::create(){
	_label->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:red;}");
	_label->setText(tr("Invalid input"));
	if (_price->hasAcceptableInput()){
		QString txt =_price->text();
		int price = txt.toInt();
		if (_participants->hasAcceptableInput()){
        	QString text = _participants->text();
        	int nb = text.toInt();
        	if (nb==2||nb==4||nb==8||nb==16||nb==32){

                _client->sendTournamentCreation(nb, price);
                int result = _client->getConfirmation();
                if(result != 0){
                    _label->setText(tr("Tournament created !"));
                    _label->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:white;}");
                }

        	}
        	else {
        		_label->setText(tr("Number of participants must be a power of 2"));
        	}
		}
	}
	update();
}

void TournamentsWidget::join(){
	_join->setEnabled(false);
	_hasJoined=true;
	_client->askToJoinTournament();
    int confirmation = _client->getConfirmation();
    if(confirmation == 0){
    	_label->setText(tr("Impossible to join this tournament !"));
    	_label->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:red;}");
    }else{
		_label->setText(tr("You have joined this tournament. Be ready."));
		_label->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:white;}");
    }
    update();
    _label->setVisible(true);
}

void TournamentsWidget::updateLabels(){
	std::vector<int> infos = _client->getTournamentList();
	if (infos.size()!=0){
		_listTournaments->setVisible(true);
		_listTournaments->clear();
		_listTournaments->setCurrentRow(0);
		if (!_hasJoined && _role==NORMAL_LOGIN) _join->setEnabled(true);
		if (_role==ADMIN_LOGIN) _create->setEnabled(false);
		for (int i=0;i<infos.size();i+=3){
			_listTournaments->addItem(tr("Tournament"));
			_currentParticipants->setText(tr("Participants : %1/%2").arg(infos[i+1]).arg(infos[i]));
			_startingPrice->setText(tr("Starting price : %1 gold").arg(infos[i+2]));
			update();
		}
	}
	else{
		_currentParticipants->setText("");
		_label->setText("");
		_startingPrice->setText("");
		_listTournaments->clear();
		maskLabel();
		_hasJoined=false;
		_listTournaments->setVisible(false);
		if (_role==ADMIN_LOGIN) _create->setEnabled(true);
	}
}