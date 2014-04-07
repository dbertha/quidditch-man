#include "ManagePlayerWidget.hpp"

ManagePlayerWidget::ManagePlayerWidget(Client* client, QWidget* parent) : _client(client),_parent(parent){
	
	setStyleSheet("ManagePlayerWidget{ background-color: rgba(255, 255, 255, 130);}");

	_listPlayersWidget = new ListPlayersWidget(_client,this);


	_capacity = new QListWidget();
	_capacity->setMaximumHeight(40);
	_capacity->setMaximumWidth(150);

	_capacity->addItem(QString("Speed"));
	_capacity->addItem(QString("Strength"));
	_capacity->addItem(QString("Precision"));
	_capacity->addItem(QString("Reflex"));
	_capacity->addItem(QString("Resistance"));
	_capacity->setCurrentRow(0);

	_trainButton = new QPushButton(tr("Train"));
	_healButton = new QPushButton(tr("Heal"));
	_sellButton = new QPushButton(tr("Sell"));
	_trainButton->setMaximumWidth(45);
	_healButton->setMaximumWidth(45);
	_sellButton->setMaximumWidth(45);

	_stack = new QStackedWidget();

	//_stack->addWidget(_zone);
	//_stack->setCurrentWidget(_zone);

	_priceToSell = new QLineEdit();
	QIntValidator* validator = new QIntValidator(10, 9999999, this);
	_priceToSell->setValidator(validator);

	_errorLabel = new QLabel("Player blocked or not enough action points");
	_errorLabel->setVisible(false);

    QWidget *panel = new QWidget(this);
	 
	QGridLayout *grid = new QGridLayout(this);
	panel->setLayout(grid);
	

	grid->setRowMinimumHeight(0,340);
	grid->setRowMinimumHeight(1,30);
	grid->setRowMinimumHeight(2,30);
	grid->setColumnMinimumWidth(0,45);
	grid->setColumnMinimumWidth(1,45);
	grid->setColumnMinimumWidth(2,45);
	grid->setColumnMinimumWidth(3,135);
	grid->setColumnMinimumWidth(4,45);

	grid->setVerticalSpacing(0);
	grid->setHorizontalSpacing(0);

	grid->addWidget(_listPlayersWidget,0,0,1,5,Qt::AlignCenter);
	grid->addWidget(_capacity,1,0,Qt::AlignRight);
	grid->addWidget(_trainButton,1,1,Qt::AlignLeft);
	grid->addWidget(_healButton,1,2,Qt::AlignCenter);
	grid->addWidget(_priceToSell,1,3,Qt::AlignRight);
	grid->addWidget(_sellButton,1,4,Qt::AlignLeft);
	grid->addWidget(_errorLabel,2,0,1,5,Qt::AlignCenter);

	setFixedSize(520,440);

	connect(_trainButton,SIGNAL(clicked()),this,SLOT(train()));
	connect(_healButton,SIGNAL(clicked()),this,SLOT(heal()));
	connect(_sellButton,SIGNAL(clicked()),this,SLOT(sell()));
}

void ManagePlayerWidget::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void ManagePlayerWidget::pause() {_listPlayersWidget->pause();}
void ManagePlayerWidget::resume() {_listPlayersWidget->resume();}

void ManagePlayerWidget::train() {
	if (_listPlayersWidget->getCurrentPlayer()!=-1&&_capacity->currentRow()!=-1){
		_client->trainPlayer(_listPlayersWidget->getCurrentPlayer(),_capacity->currentRow());
		bool result = _client->getConfirmation();
		if (result) {
			std::vector<int> trainingCenterInfos = _client->receiveBuildingInfos(TRAININGCENTER);
			_errorLabel->setText(QString("Training will be over in %1 minute(s)").arg(trainingCenterInfos[2]));
			_errorLabel->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:white;}");
		}
		else {
		_errorLabel->setText(QString("Player blocked or not enough action points"));
		_errorLabel->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:red;}");
		}
	}
	else {
		_errorLabel->setText(QString("No capacity selected"));
		_errorLabel->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:red;}");
	}
	_errorLabel->setVisible(true);
}
void ManagePlayerWidget::heal() {
	if (_listPlayersWidget->getCurrentPlayer()!=-1){
		_client->healPlayer(_listPlayersWidget->getCurrentPlayer());
		bool result = _client->getConfirmation();
		if (result) {
			std::vector<int> hospitalInfos = _client->receiveBuildingInfos(HOSPITAL);
			_errorLabel->setText(QString("The player will leave the hospital in %1 minute(s)").arg(hospitalInfos[2]));
			_errorLabel->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:white;}");
		}
		else {
		_errorLabel->setText(QString("Player blocked or in full health or not enough action points"));
		_errorLabel->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:red;}");
		}
	}
	else {
		_errorLabel->setText(QString("No capacity selected"));
		_errorLabel->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:red;}");
	}
	_errorLabel->setVisible(true);
}

void ManagePlayerWidget::sell() {
	if (_listPlayersWidget->getCurrentPlayer()!=-1){

		if (_priceToSell->hasAcceptableInput()) {
	        QString text = _priceToSell->text();
	        int startingPrice = text.toInt();

	        _client->sellPlayer(_listPlayersWidget->getCurrentPlayer(),startingPrice);
			bool result = _client->getConfirmation();
			if (result) {
				_errorLabel->setText(QString("Auction succesfully created"));
				_errorLabel->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:white;}");
			}
			else {
				_errorLabel->setText(QString("Player blocked or not enough action points"));
				_errorLabel->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:red;}");
			}
		}

		else {

		_errorLabel->setText(QString("Invalid input"));
		_errorLabel->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:red;}");
		}
	}
	else {
		_errorLabel->setText(QString("No capacity selected"));
		_errorLabel->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:red;}");
	}
	_errorLabel->setVisible(true);
}

void ManagePlayerWidget::maskLabel(){
	_errorLabel->setVisible(false);
}