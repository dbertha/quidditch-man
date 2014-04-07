#include "ListAuctionsWidget.hpp"

ListAuctionsWidget::ListAuctionsWidget(Client* client, QWidget* parent) : _client(client),_parent(parent){
	
	setStyleSheet("ListAuctionsWidget{background-image: url(Pictures/transparent.png);}");


	_noAuctionLabel = new QLabel("There isn't any auction available");
	_noAuctionLabel->setStyleSheet("QLabel{background-color: rgba(0,0,0,30);font: 16px \"Elegant Thin\", sans-serif;color:red;}");
	_noAuctionLabel->setAlignment(Qt::AlignCenter);
	std::vector<std::string> names = _client->receiveAuctionsList();

	_stack = new QStackedWidget();
	_stack->addWidget(_noAuctionLabel);

	_listAuctions = new QListWidget();
	_listAuctions->setMinimumHeight(40);
	_listAuctions->setMaximumHeight(40);

	for (unsigned i=0;i<names.size();i+=4){
		std::string tmp=names[i+1]+" | Price "+names[i+3];
		_auctionsTitle.push_back(tmp);
		QString name= QString(tr(tmp.c_str()));
		_listAuctions->addItem(name);
		int auctionID = tr(names[i].c_str()).toInt();
		_auctionsID.push_back(auctionID);
		_auctionsStartingPrice.push_back(tr(names[i+3].c_str()).toInt());
		_auctions.push_back(new PlayerWidget(_client,this,name,auctionID,true));
		_stack->addWidget(_auctions[i/4]);
	}
	if (_auctionsTitle.size()!=0){
		_listAuctions->setCurrentRow(0);
		_stack->setCurrentWidget(_auctions[0]);
	}
	else{
		_stack->setCurrentWidget(_noAuctionLabel);
	}

	if (_listAuctions->count()!=0){
		_noAuctionLabel->setText(tr("Select an auction"));
	}

    QWidget *panel = new QWidget(this);
	 
	QGridLayout *grid = new QGridLayout(this);
	panel->setLayout(grid);
	

	grid->setRowMinimumHeight(0,30);
	grid->setRowMinimumHeight(1,270);
	grid->setColumnMinimumWidth(0,500);

	grid->setVerticalSpacing(0);
	grid->setHorizontalSpacing(0);

	grid->addWidget(_listAuctions,0,0,Qt::AlignCenter);
	grid->addWidget(_stack,1,0,Qt::AlignCenter);

	_timer = new QTimer();
	connect(_timer, SIGNAL(timeout()),this,SLOT(update()));
	_timer->setInterval(1*1000);
	_timer->start();
	setFixedSize(500,340);

	connect(_listAuctions,SIGNAL(itemSelectionChanged()),this,SLOT(displayAuction()));
}

void ListAuctionsWidget::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void ListAuctionsWidget::pause() {_timer->stop();}
void ListAuctionsWidget::resume() {_timer->start();}

void ListAuctionsWidget::displayAuction(){
	dynamic_cast<AuctionWidget*>(_parent)->maskLabel();
	dynamic_cast<AuctionWidget*>(_parent)->makeJoinable();
	if (_listAuctions->currentRow()>=0) _stack->setCurrentWidget(_auctions[_listAuctions->currentRow()]);
	else _stack->setCurrentWidget(_noAuctionLabel);

	if (_listAuctions->count()==0) {
		_noAuctionLabel->setText(tr("There isn't any auction available"));
		update();
		_stack->setCurrentWidget(_noAuctionLabel);
	}
}

void ListAuctionsWidget::update(){
	show();
	std::vector<std::string> names =  _client->receiveAuctionsList();
	std::vector<std::string> tmpList;
	_auctionsID.clear();
	_auctionsStartingPrice.clear();
	for (int i=0;i<names.size();i+=4){
		std::string tmp=names[i+1]+" | Price "+names[i+3];
		tmpList.push_back(tmp);
		_auctionsID.push_back(tr(names[i].c_str()).toInt());
		_auctionsStartingPrice.push_back(tr(names[i+3].c_str()).toInt());
	}
	bool alreadyHere,stillHere;
	for (unsigned i=0;i<names.size();i+=4){
		alreadyHere=false;
		for (unsigned j=0;j<_auctionsTitle.size();++j){
			if (tmpList[i/4]==_auctionsTitle[j]){
				alreadyHere=true;
				_auctions[j]->updateLabels();
			}
		}
		if (!alreadyHere){
			QString name= QString(tr(tmpList[i/4].c_str()));
			
			_auctions.push_back(new PlayerWidget(_client,this,name,tr(names[i].c_str()).toInt(),true));
			_stack->addWidget(_auctions.back());
			_listAuctions->addItem(name);
		}
	}
	int nbRemoved=0;
	for (unsigned i=0;i<_auctionsTitle.size();++i){
		stillHere=false;
		for (unsigned j=0;j<tmpList.size();++j){
			if (_auctionsTitle[i]==tmpList[j]){
				stillHere=true;
			}
		}
		if (!stillHere){
			if (_listAuctions->count()==1){
				_stack->setCurrentWidget(_noAuctionLabel);
			}

			_listAuctions->takeItem(i-nbRemoved); 
			
			_stack->removeWidget(_auctions[i-nbRemoved]);
			delete _auctions[i-nbRemoved];
			_auctions.erase(_auctions.begin()+i-nbRemoved);
			
			++nbRemoved;

		}
	}
	_auctionsTitle=tmpList;
	if (_listAuctions->count()!=0){
		_noAuctionLabel->setText(tr("Select an auction"));
	}
	else {

		_noAuctionLabel->setText(tr("There isn't any auction available"));
	}
	
}


int ListAuctionsWidget::getAuctionID(){
	if (_listAuctions->currentRow()>=0) return _auctionsID[_listAuctions->currentRow()];
	else return -1;
}

int ListAuctionsWidget::getAuctionStartingPrice(){
	if (_listAuctions->currentRow()>=0) return _auctionsStartingPrice[_listAuctions->currentRow()];
	else return -1;
}

void ListAuctionsWidget::hide(){
	_listAuctions->setVisible(false);
}

void ListAuctionsWidget::show(){
	_listAuctions->setVisible(true);
}