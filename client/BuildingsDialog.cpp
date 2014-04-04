#include "BuildingsDialog.hpp"

BuildingsDialog::BuildingsDialog(Client * client, MainGui *parent)
    : QDialog(parent),parent_(parent), row(-1), myTimerId(0), __client(client) {
    setWindowTitle(tr("Buildings"));
    buildingsModel=new BuildingsModel(this);
    hAxe<<"Current level"<<"Upgrade cost"<<"Max. seats"<<"Upgrading";
    vAxe<<"Stadium"<<"Training center"<<"Hospital"<<"Fanshop"<<"Promotion center";
    buildingsModel->setAxes(hAxe,vAxe);
    loadedData.resize(vAxe.length());
//    i=0;

    tableView = new QTableView(this);
    tableView->setModel(buildingsModel);
    tableView->setAlternatingRowColors(true);
    selectionModel = tableView->selectionModel();

    selectButton = new QPushButton(tr("Select this building to upgrade"));
    selectButton->setDefault(true);
    cancelButton= new QPushButton(tr("Cancel"));
    bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(selectButton);
    bottomLayout->addWidget(cancelButton);
    connect(selectButton, SIGNAL(clicked()), this, SLOT(select()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tableView);
    mainLayout->addLayout(bottomLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);
    setMinimumWidth(550);
}

void BuildingsDialog::select() {
    //interacts when the selectButton is clicked
    row = selectionModel->currentIndex().row();
    if(row<0 || row >=vAxe.length()) return;
    buildingInfos = __client->receiveBuildingInfos(row+1);
    //is the cost not too high ?
    //is this building not undergoing an upgrade ?
    //has the manager enough action points ?
    if ((parent_->getMoney()>=buildingInfos[1])&&(buildingInfos[3]==0)&&
            (parent_->getActionPoints()>=AP_UPGRADE)) {
        str ="Are you sure to upgrade "+vAxe.at(row)+" ?";
        if(QMessageBox::question(this,tr("Upgrade"),str,
           QMessageBox::Ok | QMessageBox::Cancel)!=QMessageBox::Ok) return;
        __client->askForBuildingUpgrade(row+1);
        if (__client->getConfirmation()) {
            str="This building has started upgrading !\n"
                    "Upgrade will be complete in "+
                    QString::number((1+buildingInfos[0])*TIMESCALE)+" minutes !";
            QMessageBox::information(this,tr("Upgrade"),str,QMessageBox::Ok);
            return showInfo();
        }
    }
    str="You can't upgrade this building !\n"
             "(not enough money, not enough actions points or this building is already upgrading)";
    QMessageBox::critical(this,tr("Upgrade"),str,QMessageBox::Ok);
}
void BuildingsDialog::showInfo() {
    //updates the data presented by the model
//    std::cout<<"**buildingsDialog "<<i<<" ";
    for (int j=0;j<vAxe.length();++j) {
        buildingInfos = __client->receiveBuildingInfos(j+1);
        loadedData[j].clear();
        for (int k=0;k<hAxe.length()-1;++k) {
            if (k!=2 || (j==0 || j==3)) {
                QString str = QString::number(buildingInfos[k]);
                loadedData[j]<<str;//seats available
            }
            else loadedData[j]<<" ";
        }
        if (buildingInfos[3]==1) loadedData[j]<<"v";// upgrading
        else loadedData[j]<<" ";
    }
    buildingsModel->loadData(&loadedData);
//    ++i;
}

void BuildingsDialog::paintEvent(QPaintEvent *) {
    QPainter painter(this);
}

void BuildingsDialog::showEvent(QShowEvent *) {
    //starts the timer when the widget is visible
    showInfo();
    myTimerId = startTimer(15000);
}

void BuildingsDialog::timerEvent(QTimerEvent *event) {
    //checks first it's about our event !
    if (event->timerId() == myTimerId) showInfo();
    else QWidget::timerEvent(event);
    //it's not about our event - let's give it to our base class
}

void BuildingsDialog::hideEvent(QHideEvent *) {
    //timer stops when widget is hidden
    killTimer(myTimerId);
    myTimerId = 0;
}
