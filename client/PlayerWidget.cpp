#include "PlayerWidget.hpp"

PlayerWidget::PlayerWidget(Client* client, QWidget* parent,QString name,int playerID,bool auction) : _client(client),\
_parent(parent),_name(name),_playerID(playerID),_auction(auction){
	
	setStyleSheet("PlayerWidget{background-image: url(Pictures/transparent.png);}");

	if (!_auction) _infos=_client->receivePlayerInfo(_playerID);
	else _infos=_client->receiveAuctionPlayerInfo(_playerID);


	_nameLabel = new QLabel(_name);
	_nameLabel->setStyleSheet("QLabel{font: 20px \"Elegant Thin\", sans-serif;color: #53502d;}");

	_speed = new QLabel(QString("Speed : %1 | Training left : %2").arg(_infos[0]).arg(_infos[5]));
	_strength = new QLabel(QString("Strength : %1 | Training left : %2").arg(_infos[1]).arg(_infos[6]));
	_precision = new QLabel(QString("Precision : %1 | Training left : %2").arg(_infos[2]).arg(_infos[7]));
	_reflex = new QLabel(QString("Reflex : %1 | Training left : %2").arg(_infos[3]).arg(_infos[8]));
	_resistance = new QLabel(QString("Resistance : %1 | Training left : %2").arg(_infos[4]).arg(_infos[9]));
	_life = new QLabel(QString("Life : %1/%2").arg(_infos[13]).arg(_infos[4]));
	QString blockMsg;
	if (!_auction){
		if (_infos[10]==1) blockMsg = QString("This player is currently blocked");
		else blockMsg = QString("This player is not blocked");
	}
	else {
		_client->askAuctionTimeLeft(_playerID);
        int auctionTimeLeft=_client->getAuctionTimeLeft();
        blockMsg = QString("The auction starts in %1 seconds").arg(auctionTimeLeft);
        if (auctionTimeLeft<=1) blockMsg = QString("The auction starts in %1 second").arg(auctionTimeLeft);
	}
	_blocked = new QLabel(blockMsg);
	_estimatedValue = new QLabel(QString("Player estimated value : %1").arg(_infos[14]));

	_speed->setAlignment(Qt::AlignRight);
	_strength->setAlignment(Qt::AlignRight);
	_precision->setAlignment(Qt::AlignRight);
	_reflex->setAlignment(Qt::AlignRight);
	_resistance->setAlignment(Qt::AlignRight);
	_life->setAlignment(Qt::AlignRight);
	_blocked->setAlignment(Qt::AlignRight);
	_estimatedValue->setAlignment(Qt::AlignRight);

	_speed->setStyleSheet("QLabel{font: 19px \"Elegant Thin\", sans-serif;color: #53502d;}");
	_strength->setStyleSheet("QLabel{font: 19px \"Elegant Thin\", sans-serif;color: #53502d;}");
	_precision->setStyleSheet("QLabel{font: 19px \"Elegant Thin\", sans-serif;color: #53502d;}");
	_reflex->setStyleSheet("QLabel{font: 19px \"Elegant Thin\", sans-serif;color: #53502d;}");
	_resistance->setStyleSheet("QLabel{font: 19px \"Elegant Thin\", sans-serif;color: #53502d;}");
	_life->setStyleSheet("QLabel{font: 19px \"Elegant Thin\", sans-serif;color: #53502d;}");
	_blocked->setStyleSheet("QLabel{font: 19px \"Elegant Thin\", sans-serif;color: #53502d;}");
	_estimatedValue->setStyleSheet("QLabel{font: 19px \"Elegant Thin\", sans-serif;color: #53502d;}");

    QWidget *panel = new QWidget(this);
	 
	QGridLayout *grid = new QGridLayout(this);
	panel->setLayout(grid);
	
	grid->addWidget(_nameLabel,0,0,Qt::AlignLeft);
	grid->addWidget(_speed, 1,0, Qt::AlignLeft );
	grid->addWidget(_strength, 2,0, Qt::AlignLeft );
	grid->addWidget(_precision, 3,0, Qt::AlignLeft );
	grid->addWidget(_reflex, 4,0, Qt::AlignLeft );
	grid->addWidget(_resistance, 5,0, Qt::AlignLeft );
	grid->addWidget(_life, 6,0, Qt::AlignLeft );
	grid->addWidget(_blocked, 8,0, Qt::AlignLeft );
	grid->addWidget(_estimatedValue, 7,0, Qt::AlignLeft );
	//580-780
	//220
	grid->setVerticalSpacing(0);
	grid->setHorizontalSpacing(0);
			//connect(navButton, SIGNAL(clicked()), mapper, SLOT(map()));

	for (int i=0;i<9;++i){
		grid->setRowMinimumHeight(i,30);
	}
	grid->setColumnMinimumWidth(0,500);
	
	//grid->setRowMinimumHeight(0,30);
	/*
	grid->setRowMinimumHeight(1,100);
	grid->setRowMinimumHeight(2,70);
	for (int i=3;i<=5;++i) {
		grid->setRowMinimumHeight(i,30);
	}
	*/
/*
	_upgradeButton = new QPushButton(tr("Upgrade"));
	_upgradeButton->setStyleSheet("QButton{font: 19px \"Elegant Thin\", sans-serif;color: #53502d;}");
	grid->addWidget(_upgradeButton,6,0,Qt::AlignCenter);
	if (_infos[3]==1) _upgradeButton->setEnabled(false);
	_errorLabel = new QLabel();
	_errorLabel->setVisible(false);
	grid->addWidget(_errorLabel,5,0,Qt::AlignCenter);
	_timer = new QTimer();
	connect(_timer, SIGNAL(timeout()),this,SLOT(updateLabels()));
	_timer->setInterval(2*1000);
	_timer->start();
*/
	setFixedSize(500,270);

	//connect(_upgradeButton,SIGNAL(clicked()),this,SLOT(upgrade()));
}

void PlayerWidget::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void PlayerWidget::pause() {}
void PlayerWidget::resume() {}

void PlayerWidget::updateLabels(){

	
	if (!_auction) _infos=_client->receivePlayerInfo(_playerID);
	else _infos=_client->receiveAuctionPlayerInfo(_playerID);

	_speed->setText(QString("Speed : %1 | Training left : %2").arg(_infos[0]).arg(_infos[5]));
	_strength->setText(QString("Strength : %1 | Training left : %2").arg(_infos[1]).arg(_infos[6]));
	_precision ->setText(QString("Precision : %1 | Training left : %2").arg(_infos[2]).arg(_infos[7]));
	_reflex->setText(QString("Reflex : %1 | Training left : %2").arg(_infos[3]).arg(_infos[8]));
	_resistance->setText(QString("Resistance : %1 | Training left : %2").arg(_infos[4]).arg(_infos[9]));
	_life->setText(QString("Life : %1/%2").arg(_infos[13]).arg(_infos[4]));
	QString blockMsg;
	if (!_auction){
		if (_infos[10]==1) blockMsg = QString("This player is currently blocked");
		else blockMsg = QString("This player is not blocked");
	}
	else {
		_client->askAuctionTimeLeft(_playerID);
        int auctionTimeLeft=_client->getAuctionTimeLeft();
        blockMsg = QString("The auction starts in %1 seconds").arg(auctionTimeLeft);
        if (auctionTimeLeft<=1) blockMsg = QString("The auction starts in %1 second").arg(auctionTimeLeft);
	}
	_blocked->setText(blockMsg);
	_estimatedValue->setText(QString("Player estimated value : %1").arg(_infos[14]));
}

void PlayerWidget::setNewID(int playerID) {_playerID=playerID;}
