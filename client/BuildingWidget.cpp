#include "BuildingWidget.hpp"

BuildingWidget::BuildingWidget(Client* client, QWidget* parent,int buildingID) : _client(client),_parent(parent),_buildingID(buildingID){
	
	setStyleSheet("BuildingWidget{ background-color: rgba(255, 255, 255, 100);}");

	_infos=_client->receiveBuildingInfos(_buildingID);
	_names.push_back("STADIUM");
	_names.push_back("TRAINING CENTER");
	_names.push_back("HOSPITAL");
	_names.push_back("FAN SHOP");
	_names.push_back("PROMOTION CENTER");

	_name = new QLabel(tr(_names[_buildingID-1].c_str()));
	_name->setStyleSheet("QLabel{font: 20px \"Elegant Thin\", sans-serif;color: #53502d;}");

	QString level = QString("Level : %1").arg(_infos[0]);
	QString price = QString("Price to upgrade : %1 gold").arg(_infos[1]);
	QString attribute;
	if (_buildingID==STADIUM) attribute = QString("Max places in the stadium : %1").arg(_infos[2]);
	else if (_buildingID==TRAININGCENTER||_buildingID==HOSPITAL) attribute = QString("Time of block : %1").arg(_infos[2]);
	else if (_buildingID==FANSHOP) attribute = QString("Max clients per match : %1").arg(_infos[2]);
	else if (_buildingID==PROMOTIONCENTER) attribute = QString("AP gained by waiting %2 minutes : %1").arg(_infos[2]).arg(TIMESCALEACTIONPOINTS);
	QString isUpgrading;
	if (_infos[3]!=1) isUpgrading = QString("This building is not under construction");
	else isUpgrading = QString("This building is currently under construction");

	_levelLabel = new QLabel(level);
	_priceLabel = new QLabel(price);
	_attributeLabel = new QLabel(attribute);
	_isUpgradingLabel = new QLabel(isUpgrading);

	_levelLabel->setAlignment(Qt::AlignRight);
	_priceLabel->setAlignment(Qt::AlignRight);
	_attributeLabel->setAlignment(Qt::AlignRight);
	_isUpgradingLabel->setAlignment(Qt::AlignRight);

	_levelLabel->setStyleSheet("QLabel{font: 19px \"Elegant Thin\", sans-serif;color: #53502d;}");
	_priceLabel->setStyleSheet("QLabel{font: 19px \"Elegant Thin\", sans-serif;color: #53502d;}");
	_attributeLabel->setStyleSheet("QLabel{font: 19px \"Elegant Thin\", sans-serif;color: #53502d;}");
	_isUpgradingLabel->setStyleSheet("QLabel{font: 19px \"Elegant Thin\", sans-serif;color: #53502d;}");

    QWidget *panel = new QWidget(this);
	 
	QGridLayout *grid = new QGridLayout(this);
	panel->setLayout(grid);
	
	grid->addWidget(_name,0,0,Qt::AlignCenter);
	grid->addWidget(_levelLabel, 1,0, Qt::AlignLeft );
	grid->addWidget(_priceLabel, 2,0, Qt::AlignLeft );
	grid->addWidget(_attributeLabel, 3,0, Qt::AlignLeft );
	grid->addWidget(_isUpgradingLabel, 4,0, Qt::AlignLeft );
	//580-780
	//220
	grid->setVerticalSpacing(0);
	grid->setHorizontalSpacing(0);
			//connect(navButton, SIGNAL(clicked()), mapper, SLOT(map()));

	for (int i=0;i<7;++i){
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

	setFixedSize(500,220);

	connect(_upgradeButton,SIGNAL(clicked()),this,SLOT(upgrade()));
}

void BuildingWidget::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void BuildingWidget::pause() {_timer->stop();}
void BuildingWidget::resume() {_timer->start();}

void BuildingWidget::updateLabels(){
	_infos=_client->receiveBuildingInfos(_buildingID);

	QString level = QString("Level : %1").arg(_infos[0]);
	QString price = QString("Price to upgrade : %1 gold").arg(_infos[1]);
	QString attribute;
	if (_buildingID==STADIUM) attribute = QString("Max places in the stadium : %1").arg(_infos[2]);
	else if (_buildingID==TRAININGCENTER||_buildingID==HOSPITAL) attribute = QString("Time of block : %1").arg(_infos[2]);
	else if (_buildingID==FANSHOP) attribute = QString("Max clients per match : %1").arg(_infos[2]);
	else if (_buildingID==PROMOTIONCENTER) attribute = QString("AP gained by waiting 3 seconds : %1").arg(_infos[2]);
	QString isUpgrading;
	if (_infos[3]==0) isUpgrading = QString("This building is not under construction");
	else isUpgrading = QString("This building is currently under construction");

	_levelLabel->setText(level);
	_priceLabel->setText(price);
	_attributeLabel->setText(attribute);
	_isUpgradingLabel->setText(isUpgrading);

	if (_infos[3]==0) {
		_upgradeButton->setEnabled(true);
		if (_upgrading==1){
			_upgrading=0;
			_errorLabel->setVisible(false);
		}
	}

	if (_infos[3]==1) _upgradeButton->setEnabled(false);
	update();
}

void BuildingWidget::upgrade(){
	_client->askForBuildingUpgrade(_buildingID);
	if (_client->getConfirmation()) {
            QString confirm ="Upgrade has started and will be complete in "+
                    QString::number((1+_infos[0])*TIMESCALE)+" minutes !";
            _errorLabel->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:white;}");
            _errorLabel->setText(confirm);
            _upgrading=1;
        }
   
    else {
    	QString error = "Not enough money or action points (10 needed).";
    	_errorLabel->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:red;}");
    	_errorLabel->setText(error);
    }
    _errorLabel->setVisible(true);
    update();
}

void BuildingWidget::maskLabel(){
	_errorLabel->setVisible(false);
}