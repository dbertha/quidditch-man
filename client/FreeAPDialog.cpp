#include "FreeAPDialog.hpp"

FreeAPDialog::FreeAPDialog(Client * client, QWidget *parent): QDialog(parent), __client(client)
{



    std::string msg = "You have started a promotion campaign. ";
    std::string msg2 ="(You will gain AP for every " + intToString(TIMESCALEACTIONPOINTS) +" minutes spent in here) ";
    label = new QLabel(tr(msg.c_str()));
    label2 = new QLabel(tr(msg2.c_str()));
    
    closeButton = new QPushButton(tr("Quit"));

    connect(closeButton, SIGNAL(clicked()),
            this, SLOT(stopCampaign()));

    QVBoxLayout *topLeftLayout = new QVBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(label2);

    QHBoxLayout *rightLayout = new QHBoxLayout;
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(topLeftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    errorMessageDialog = new QErrorMessage(this);

    setWindowTitle(tr("Get free action points"));
    setFixedHeight(sizeHint().height());
    init();
}

void FreeAPDialog::init() {
    label->setText(tr("You have started a promotion campaign. "));
    __client->startPromotionCampaign();
    setResult(Rejected);

}

void FreeAPDialog::stopCampaign()
{
    if (__client->endPromotionCampaign()!=0) {
        int result = __client->getPromotionResult();
        QMessageBox msgBox;
        QString msg = "You have gained "+QString::number(result)+" action points.";
        msgBox.setText(msg);
        msgBox.exec();
    }
    reject();
    
}
