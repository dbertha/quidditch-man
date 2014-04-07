#include "NotificationWidget.hpp"
#include "MatchWindow.hpp"

NotificationWidget::NotificationWidget(Client* client, MainWindow* parent,SelectPlayersWidget* select) :\
_client(client),_parent(parent),_select(select){
	
	setStyleSheet("NotificationWidget{background-color: rgba(0,0,0,222);}");
	setVisible(false);
	_label = new QLabel("");

	_label->setAlignment(Qt::AlignCenter);

	_label->setStyleSheet("QLabel{font: 15px \"Elegant Thin\", sans-serif;color: #c5c295;}");

    QWidget *panel = new QWidget(this);
	 
	grid = new QGridLayout(this);
	panel->setLayout(grid);

	_okButton = new QPushButton(tr("Ok"));
	_yesButton = new QPushButton(tr("Yes"));
	_noButton = new QPushButton(tr("No"));
	_quitButton = new QPushButton(tr("Quit"));
	_backButton = new QPushButton(tr("Back"));
	
	grid->addWidget(_label, 1,0,1,3, Qt::AlignCenter );
	grid->addWidget(_okButton, 2,1, Qt::AlignCenter);
	grid->addWidget(_yesButton, 2,0, Qt::AlignRight);
	grid->addWidget(_noButton, 2,2, Qt::AlignLeft);
	grid->addWidget(_quitButton, 2,1,Qt::AlignCenter);
	grid->addWidget(_backButton, 2,2,Qt::AlignLeft);
	//580-780
	//220
	grid->setHorizontalSpacing(0);
	grid->setVerticalSpacing(0);
			//connect(navButton, SIGNAL(clicked()), mapper, SLOT(map()));

	grid->setRowMinimumHeight(0,45);
	grid->setRowMinimumHeight(1,30);
	grid->setRowMinimumHeight(2,30);
	grid->setRowMinimumHeight(3,30);
	grid->setColumnMinimumWidth(0,180);
	grid->setColumnMinimumWidth(1,180);
	grid->setColumnMinimumWidth(2,180);
	
	//grid->setRowMinimumHeight(0,30);
	/*
	grid->setRowMinimumHeight(1,100);
	grid->setRowMinimumHeight(2,70);
	for (int i=3;i<=5;++i) {
		grid->setRowMinimumHeight(i,30);
	}
	*/
	setFixedSize(540,135);

	connect(_okButton,SIGNAL(clicked()),this,SLOT(startMatch()));
	connect(_yesButton,SIGNAL(clicked()),this,SLOT(acceptFriendlyMatch()));
	connect(_noButton,SIGNAL(clicked()),this,SLOT(refuseFriendlyMatch()));
	connect(_backButton,SIGNAL(clicked()),this,SLOT(back()));
	connect(_quitButton,SIGNAL(clicked()),this,SLOT(quit()));
}

void NotificationWidget::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void NotificationWidget::acceptFriendlyMatch() {
	setVisible(false);
	_select->init(true,-1);
}

void NotificationWidget::refuseFriendlyMatch() {
	setVisible(false);
	std::vector<int> team;
	_parent->pause();
	_client->answerMatchProposal(false, team);
    //~ answerMatchProposal(confirmation, playersInTeam); //liste vide = refus de l'invitation
    if(_client->receiveMatchConfirmation() == MATCH_STARTING){
        MatchWindow * matchWindow  = new MatchWindow(_client, 2, _parent);
        matchWindow->show();
    }
    _parent->hideSelect();
    _parent->resume();
    _parent->deblock();
}


void NotificationWidget::startMatch() {
	setVisible(false);
	//std::vector<int> playersInTeam = chooseTeamForMatch(_client, this);
	_select->init(false,0);
	//_parent->resume();
}

void NotificationWidget::quit(){
	_parent->quit();
}

void NotificationWidget::back(){
	setVisible(false);
	//_parent->resume();
	_parent->deblock();
}

void NotificationWidget::tournamentNotification(char name[], int opponentID){
	_parent->block();
	_parent->hideSelect();
	_parent->hide();
	_yesButton->setVisible(false);
	_noButton->setVisible(false);
	_okButton->setVisible(true);
	_backButton->setVisible(false);
	_quitButton->setVisible(false);
	QString text = QString("%1, player with ID %2 is your opponent in the tournament!").arg(name, QString::number(opponentID));
	_label->setText(text);
	update();
	setVisible(true);
}

void NotificationWidget::friendlyMatchNotification(char name[], int opponentID){
	_parent->block();
	_parent->hideSelect();
	_parent->hide();
	_parent->hide();
	_yesButton->setVisible(true);
	_noButton->setVisible(true);
	_okButton->setVisible(false);
	_backButton->setVisible(false);
	_quitButton->setVisible(false);
	QString text = QString("%1, player with ID %2 wants to play a match against you !").arg(name, QString::number(opponentID));
	_label->setText(text);
	update();
	setVisible(true);
}

void NotificationWidget::trainingMatchImpossibleNotification(){
	_parent->block();
	_parent->hideSelect();
	_parent->hide();
	_yesButton->setVisible(false);
	_noButton->setVisible(false);
	_okButton->setVisible(false);
	_backButton->setVisible(true);
	_quitButton->setVisible(false);
	QString text = QString("You currently cannot play a training match.");
	_label->setText(text);
	update();
	setVisible(true);
}

void NotificationWidget::friendlyMatchDeniedNotification(){
	_parent->block();
	_parent->hideSelect();
	_parent->hide();
	_yesButton->setVisible(false);
	_noButton->setVisible(false);
	_okButton->setVisible(false);
	_backButton->setVisible(true);
	_quitButton->setVisible(false);
	QString text = QString("Invitation denied!");
	_label->setText(text);
	update();
	setVisible(true);
}

void NotificationWidget::noConnectionNotification(){
	_parent->block();
	_parent->hideSelect();
	_parent->hide();
	_yesButton->setVisible(false);
	_noButton->setVisible(false);
	_okButton->setVisible(false);
	_backButton->setVisible(false);
	_quitButton->setVisible(true);
	QString text = QString("Server is down. Current session is lost.");
	_label->setText(text);
	update();
	setVisible(true);

}