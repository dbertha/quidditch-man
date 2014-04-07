#include "BuyAPWidget.hpp"

BuyAPWidget::BuyAPWidget(Client* client, QWidget* parent) : _client(client),_parent(parent){
	
	setStyleSheet("BuyAPWidget{ background-color: rgba(255, 255, 255, 100);}");
	QIntValidator* validator = new QIntValidator(1, 999, this);

    std::string msg ="Amount:  (1 AP = " + intToString(GOLD_FOR_ONE_AP) +" gold) ";
    _label = new QLabel(tr(msg.c_str()));
    _lineEdit = new QLineEdit();
    _lineEdit->setValidator(validator);

	//_howManyAP->setMaximumWidth(20);
	//_howManyAP->setMinimumWidth(20);

	_lineEdit->setMaximumWidth(40);
	_lineEdit->setMinimumWidth(40);
    QWidget *panel = new QWidget(this);
	 
	QGridLayout *grid = new QGridLayout(this);
	panel->setLayout(grid);
	
	//580-780
	//220
	grid->setVerticalSpacing(3);
			//connect(navButton, SIGNAL(clicked()), mapper, SLOT(map()));

	for (int i=0;i<3;++i){
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
	grid->addWidget(_label,0,0,1,2,Qt::AlignCenter);
	grid->addWidget(_lineEdit,1,0,Qt::AlignRight);
	_buyButton = new QPushButton(tr("Buy"));
	_buyButton->setStyleSheet("QButton{font: 19px \"Elegant Thin\", sans-serif;color: #53502d;}");
	grid->addWidget(_buyButton,1,1,Qt::AlignLeft);
	_errorLabel = new QLabel();
	_errorLabel->setVisible(false);
	grid->addWidget(_errorLabel,2,0,1,2,Qt::AlignCenter);
	setFixedSize(300,120);

	connect(_buyButton,SIGNAL(clicked()),this,SLOT(buy()));
}

void BuyAPWidget::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}


void BuyAPWidget::buy(){
	if (_lineEdit->hasAcceptableInput()) {
        QString text = _lineEdit->text();
        int amount = text.toInt();
		if (_client->buyActionPoints(amount)!=0){
	        int result = _client->getPriceForAP();
	        if (result<=0) {
		    	QString error = "Not enough money.";
		    	_errorLabel->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:red;}");
		    	_errorLabel->setText(error);
	        }
	        else {
	            QString msg = QString("You have bought %1 action points.").arg(amount);
	            _errorLabel->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:white;}");
	            _errorLabel->setText(msg);
	        }
	    }
	}
	else{
		QString error = "Invalid input.";
	   	_errorLabel->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:red;}");
	   	_errorLabel->setText(error);

	}
    _errorLabel->setVisible(true);
    update();
  
}

void BuyAPWidget::maskLabel(){
	_errorLabel->setVisible(false);
}