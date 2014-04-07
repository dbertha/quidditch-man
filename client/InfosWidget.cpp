#include "InfosWidget.hpp"

InfosWidget::InfosWidget(Client* client, QWidget* parent) : _client(client),_parent(parent){
	
	setStyleSheet("background-image: url(Pictures/transparent.png)");

	_client->receiveManagerInfos(&_players,&_money,&_fans,&_actionPoints);

	QString money = QString("%1").arg(_money);
	QString fans = QString("%1").arg(_fans);
	QString actionPoints = QString("%1").arg(_actionPoints);
	_moneyLabel = new QLabel(money);
	_fansLabel = new QLabel(fans);
	_actionPointsLabel = new QLabel(actionPoints);

	_moneyLabel->setAlignment(Qt::AlignRight);
	_fansLabel->setAlignment(Qt::AlignRight);
	_actionPointsLabel->setAlignment(Qt::AlignRight);

	_moneyLabel->setStyleSheet("QLabel{font: 23px \"Elegant Thin\", sans-serif;color: #c5c295;}");
	_fansLabel->setStyleSheet("QLabel{font: 23px \"Elegant Thin\", sans-serif;color: #c5c295;}");
	_actionPointsLabel->setStyleSheet("QLabel{font: 23px \"Elegant Thin\", sans-serif;color: #c5c295;}");

	_moneyIcon = new QLabel();
	_fansIcon = new QLabel();
	_actionPointsIcon = new QLabel();

	QPixmap moneyPix("Pictures/goldcoinx30.png");
	_moneyIcon->setPixmap(moneyPix);
	_moneyIcon->setFixedSize(moneyPix.size());

	QPixmap fansPix("Pictures/fansx30.png");
	_fansIcon->setPixmap(fansPix);
	_fansIcon->setFixedSize(fansPix.size());

	QPixmap apPix("Pictures/eclairx30.png");
	_actionPointsIcon->setPixmap(apPix);
	_actionPointsIcon->setFixedSize(apPix.size());
/*
	_moneyIcon->setStyleSheet("background-image: url(Pictures/goldcoin30x30.png);");
	_fansIcon->setStyleSheet("background-image: url(Pictures/fansx30.png);");
	_actionPointsIcon->setStyleSheet("background-image: url(Pictures/eclair0x30.png);");
*/
    QWidget *panel = new QWidget(this);
	 
	QGridLayout *grid = new QGridLayout(this);
	panel->setLayout(grid);
	
	grid->addWidget(_moneyLabel, 0,0, Qt::AlignRight );
	grid->addWidget(_moneyIcon, 0,2, Qt::AlignLeft );
	grid->addWidget(_fansLabel, 0,4, Qt::AlignRight );
	grid->addWidget(_fansIcon, 0,6, Qt::AlignLeft );
	grid->addWidget(_actionPointsLabel, 0,8, Qt::AlignRight );
	grid->addWidget(_actionPointsIcon, 0,10, Qt::AlignLeft );
	//580-780
	//220
	grid->setHorizontalSpacing(3);
			//connect(navButton, SIGNAL(clicked()), mapper, SLOT(map()));

	for (int i=0;i<3;++i){
		grid->setColumnMinimumWidth(0+4*i,120);
		grid->setColumnMinimumWidth(1+4*i,2);
		grid->setColumnMinimumWidth(2+4*i,38);

		grid->setColumnMinimumWidth(3+4*i,20);
	}
	
	//grid->setRowMinimumHeight(0,30);
	/*
	grid->setRowMinimumHeight(1,100);
	grid->setRowMinimumHeight(2,70);
	for (int i=3;i<=5;++i) {
		grid->setRowMinimumHeight(i,30);
	}
	*/

	_timer = new QTimer();
	connect(_timer, SIGNAL(timeout()),this,SLOT(updateLabels()));
	_timer->setInterval(2*1000);
	_timer->start();

	setFixedSize(540,100);
}

void InfosWidget::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void InfosWidget::pause() {_timer->stop();}
void InfosWidget::resume() {_timer->start();}

void InfosWidget::updateLabels(){
	_client->receiveManagerInfos(&_players,&_money,&_fans,&_actionPoints);

	QString money = QString("%1").arg(_money);
	QString fans = QString("%1").arg(_fans);
	QString actionPoints = QString("%1").arg(_actionPoints);
	_moneyLabel->setText(money);
	_fansLabel->setText(fans);
	_actionPointsLabel->setText(actionPoints);
	update();
}