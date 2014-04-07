#include "GainAPWidget.hpp"
#include <vector>

GainAPWidget::GainAPWidget(Client* client, QWidget* parent) : _client(client),_parent(parent),_time(0),_campaigning(false){
	setStyleSheet("GainAPWidget{ background-color: rgba(255, 255, 255, 100);}");
	std::vector<int> promotionCenterInfos=_client->receiveBuildingInfos(PROMOTIONCENTER);
	QString text = QString("%1 AP gained every %2 minutes").arg(promotionCenterInfos[2]).arg(TIMESCALEACTIONPOINTS);
	_ratio = new QLabel(text);
	_ratio->setStyleSheet("QLabel{font: 19px \"Elegant Thin\", sans-serif;color: #53502d;}");
	_ratio->setMinimumHeight(30);
	_ratio->setMaximumHeight(30);
	_result = new QLabel();
	_result->setVisible(false);
	_result->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:white;}");

	_timeLabel = new QLabel();
	_timeLabel->setVisible(false);
	_timeLabel->setStyleSheet("QLabel{font: 19px \"Elegant Thin\", sans-serif;color: black;}");

	_startButton = new QPushButton(tr("Start"));
	_stopButton = new QPushButton(tr("Stop"));
	_startButton->setStyleSheet("QButton{font: 19px \"Elegant Thin\", sans-serif;color: #53502d;}");
	_stopButton->setStyleSheet("QButton{font: 19px \"Elegant Thin\", sans-serif;color: #53502d;}");
	_stopButton->setEnabled(false);

    QWidget *panel = new QWidget(this);
	 
	QGridLayout *grid = new QGridLayout(this);
	panel->setLayout(grid);
	
	//580-780
	//220
	grid->setVerticalSpacing(0);
	grid->setHorizontalSpacing(0);
			//connect(navButton, SIGNAL(clicked()), mapper, SLOT(map()));

	for (int i=0;i<4;++i){
		grid->setRowMinimumHeight(i,30);
	}
	grid->setColumnMinimumWidth(0,150);
	grid->setColumnMinimumWidth(1,150);
	
	//grid->setRowMinimumHeight(0,30);
	/*
	grid->setRowMinimumHeight(1,100);
	grid->setRowMinimumHeight(2,70);
	for (int i=3;i<=5;++i) {
		grid->setRowMinimumHeight(i,30);
	}
	*/
	grid->addWidget(_ratio,0,0,1,2,Qt::AlignCenter);
	grid->addWidget(_startButton,3,0,Qt::AlignRight);
	grid->addWidget(_stopButton,3,1,Qt::AlignLeft);
	grid->addWidget(_result,2,0,1,2,Qt::AlignCenter);
	grid->addWidget(_timeLabel,1,0,1,2,Qt::AlignCenter);
	setFixedSize(300,140);

	connect(_startButton,SIGNAL(clicked()),this,SLOT(startCampaign()));
	connect(_stopButton,SIGNAL(clicked()),this,SLOT(stopCampaign()));
}

void GainAPWidget::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}


void GainAPWidget::startCampaign(){
	dynamic_cast<OfficePage*>(_parent)->blockButtons();
	_client->startPromotionCampaign();
	_campaigning=true;
	_stopButton->setEnabled(true);
	_startButton->setEnabled(false);
	_timeLabel->setVisible(true);
  	_timer= new QTimer();
  	_timer->setInterval(1000);
  	connect(_timer,SIGNAL(timeout()),this,SLOT(updateLabel()));
  	_timer->start();
}

void GainAPWidget::stopCampaign(){
	dynamic_cast<OfficePage*>(_parent)->deblockButtons();
	delete _timer;
	_time=0;
	QString txt = QString("%1").arg(_time);
	_timeLabel->setVisible(false);
	_timeLabel->setText(txt);
	_campaigning=false;
	_stopButton->setEnabled(false);
	_startButton->setEnabled(true);
    if (_client->endPromotionCampaign()!=0) {
        int result = _client->getPromotionResult();
        QString msg = "You have gained "+QString::number(result)+" action points.";
        _result->setText(msg);
        _result->setVisible(true);
    }
}

void GainAPWidget::updateLabel(){
	++_time;
	QString txt = QString("%1").arg(_time);
	_timeLabel->setText(txt);
	update();
}

void GainAPWidget::maskLabel(){
	_timeLabel->setVisible(false);
	_result->setVisible(false);
}