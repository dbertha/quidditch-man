#include "DomainPage.hpp"

DomainPage::DomainPage(Client* client, MainWindow* parent) : _client(client),_parent(parent){
	setStyleSheet("DomainPage{ background-image: url(Pictures/mainMenu.jpg);}");
	_stadium = new QPushButton(tr(" Stadium"));
	_trainingCenter = new QPushButton(tr(" Training center"));
	_hospital = new QPushButton(tr("  Hospital"));
	_fanShop = new QPushButton(tr("  Fan shop"));
	_promotionCenter = new QPushButton(tr("  Promotion center"));
	_returnButton = new QPushButton(tr("     Return"));
	/*
	 QFont font("Elegansdsdn");
	font.setItalic(true);
	font.setPointSize(18);*/
	_stadium->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 20px \"Elegant Thin\", sans-serif; color: #8f6d25;text-align: left; }");
	_trainingCenter->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 20px \"Elegant Thin\", sans-serif;color: #8f6d25;text-align: left; }");
	_hospital->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 20px \"Elegant Thin\", sans-serif;color: #8f6d25;text-align: left; }");
	_fanShop->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 20px \"Elegant Thin\", sans-serif;color: #8f6d25;text-align: left; }");
	_promotionCenter->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 20px \"Elegant Thin\", sans-serif;color: #8f6d25;text-align: left; }");
	_returnButton->setStyleSheet("QPushButton { background-image: url(Pictures/transparent.png);font: 20px \"Elegant Thin\", sans-serif;color: #8f6d25;text-align: left; }");
	
	_stadium->setFlat(true);
	_trainingCenter->setFlat(true);
	_hospital->setFlat(true);
	_fanShop->setFlat(true);
	_promotionCenter->setFlat(true);
	_returnButton->setFlat(true);


	_stadium->setCursor(Qt::PointingHandCursor);
	_trainingCenter->setCursor(Qt::PointingHandCursor);
	_hospital->setCursor(Qt::PointingHandCursor);
	_fanShop->setCursor(Qt::PointingHandCursor);
	_promotionCenter->setCursor(Qt::PointingHandCursor);
	_returnButton->setCursor(Qt::PointingHandCursor);
/*

	_stadium->setFont(font);
	_managerOffice->setFont(font);
	_domain->setFont(font);
*/
	_stadium->setMinimumWidth(200);
    _stadium->setMinimumHeight(35);
    _stadium->setMaximumWidth(200);
    _stadium->setMaximumHeight(35);
    _trainingCenter->setMinimumWidth(200);
    _trainingCenter->setMinimumHeight(35);
    _trainingCenter->setMaximumWidth(200);
    _trainingCenter->setMaximumHeight(35);
    _hospital->setMinimumWidth(200);
    _hospital->setMinimumHeight(35);
    _hospital->setMaximumWidth(200);
    _hospital->setMaximumHeight(35);
    _fanShop->setMinimumWidth(200);
    _fanShop->setMinimumHeight(35);
    _fanShop->setMaximumWidth(200);
    _fanShop->setMaximumHeight(35);
    _promotionCenter->setMinimumWidth(200);
    _promotionCenter->setMinimumHeight(35);
    _promotionCenter->setMaximumWidth(200);
    _promotionCenter->setMaximumHeight(35);
    _returnButton->setMinimumWidth(200);
    _returnButton->setMinimumHeight(35);
    _returnButton->setMaximumWidth(200);
    _returnButton->setMaximumHeight(35);


    QWidget *panel = new QWidget(this);
	 
	_grid = new QGridLayout(this);
	panel->setLayout(_grid);
	
	_grid->addWidget(_stadium, 2,2, Qt::AlignLeft );
	_grid->addWidget(_trainingCenter, 3,2, Qt::AlignLeft );
	_grid->addWidget(_hospital, 4,2, Qt::AlignLeft );
	_grid->addWidget(_fanShop, 5,2, Qt::AlignLeft );
	_grid->addWidget(_promotionCenter, 6,2, Qt::AlignLeft );
	_grid->addWidget(_returnButton,9,2,Qt::AlignLeft );

	//_infos = new InfosWidget(_client,this);
	//_grid->addWidget(_infos,0,0,1,2,Qt::AlignLeft);
	//580-780
	//220
	_grid->setVerticalSpacing(3);
	_grid->setHorizontalSpacing(0);
			//connect(navButton, SIGNAL(clicked()), mapper, SLOT(map()));

	_grid->setColumnMinimumWidth(0,50);
	_grid->setColumnMinimumWidth(1,520);
	_grid->setColumnMinimumWidth(2,120);
	_grid->setRowMinimumHeight(0,100);
	_grid->setRowMinimumHeight(1,90);
	for (int i=2;i<=12;++i) {
		_grid->setRowMinimumHeight(i,30);
	}
	_stack = new QStackedWidget(this);
	_grid->addWidget(_stack,2,1,7,1);
	for (int i=1;i<=5;++i){
		_buildingWidgets.push_back(new BuildingWidget(_client,this,i));
		_buildingWidgets[i-1]->pause();
		_stack->addWidget(_buildingWidgets[i-1]);
	}
	_stack->setVisible(false);

/*
    QVBoxLayout *_mainLayout = new QVBoxLayout;
    _mainLayout->addWidget(_registerButton);
    _mainLayout->addWidget(_loginButton);
    _mainLayout->addWidget(_closeButton);
    setLayout(_mainLayout);
*/
    setWindowTitle(tr("QUIDDITH MANAGER"));
    setFixedSize(800,640);

    connect(_stadium,SIGNAL(clicked()),this,SLOT(stadium()));
    connect(_trainingCenter,SIGNAL(clicked()),this,SLOT(trainingCenter()));
    connect(_hospital,SIGNAL(clicked()),this,SLOT(hospital()));
    connect(_fanShop,SIGNAL(clicked()),this,SLOT(fanShop()));
    connect(_promotionCenter,SIGNAL(clicked()),this,SLOT(promotionCenter()));
    connect(_returnButton,SIGNAL(clicked()),this,SLOT(returnMenu()));
}

void DomainPage::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void DomainPage::pause() {
	//_infos->pause();
	for (int i=0;i<5;++i){
		_buildingWidgets[i]->pause();
	}
}
void DomainPage::resume() {
	//_infos->resume();
	for (int i=0;i<5;++i){
		_buildingWidgets[i]->resume();
	}
}
void DomainPage::hideStack() {_stack->setVisible(false);}
void DomainPage::update() {/*_infos->updateLabels();*/}
void DomainPage::stadium(){
	_stack->setCurrentIndex(STADIUM-1);
	_stack->setVisible(true);
	dynamic_cast<BuildingWidget*>(_stack->currentWidget())->resume();
	dynamic_cast<BuildingWidget*>(_stack->currentWidget())->maskLabel();
}

void DomainPage::trainingCenter(){
	_stack->setCurrentIndex(TRAININGCENTER-1);
	_stack->setVisible(true);
	dynamic_cast<BuildingWidget*>(_stack->currentWidget())->resume();
	dynamic_cast<BuildingWidget*>(_stack->currentWidget())->maskLabel();

}

void DomainPage::hospital(){
	_stack->setCurrentIndex(HOSPITAL-1);
	_stack->setVisible(true);
	dynamic_cast<BuildingWidget*>(_stack->currentWidget())->resume();
	dynamic_cast<BuildingWidget*>(_stack->currentWidget())->maskLabel();

}

void DomainPage::fanShop(){
	_stack->setCurrentIndex(FANSHOP-1);
	_stack->setVisible(true);
	dynamic_cast<BuildingWidget*>(_stack->currentWidget())->resume();
	dynamic_cast<BuildingWidget*>(_stack->currentWidget())->maskLabel();

}

void DomainPage::promotionCenter(){
	_stack->setCurrentIndex(PROMOTIONCENTER-1);
	_stack->setVisible(true);
	dynamic_cast<BuildingWidget*>(_stack->currentWidget())->resume();
	dynamic_cast<BuildingWidget*>(_stack->currentWidget())->maskLabel();

}

void DomainPage::returnMenu(){
	//pause();
	_parent->mainPage();
}

void DomainPage::blockButtons(){
	_stadium->setEnabled(false);
	_trainingCenter->setEnabled(false);
	_hospital->setEnabled(false);
	_fanShop->setEnabled(false);
	_promotionCenter->setEnabled(false);
	_returnButton->setEnabled(false);
}

void DomainPage::deblockButtons(){
	_stadium->setEnabled(true);
	_trainingCenter->setEnabled(true);
	_hospital->setEnabled(true);
	_fanShop->setEnabled(true);
	_promotionCenter->setEnabled(true);
	_returnButton->setEnabled(true);
}