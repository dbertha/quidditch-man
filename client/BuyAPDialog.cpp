#include "BuyAPDialog.hpp"

BuyAPDialog::BuyAPDialog(Client * client, QWidget *parent): QDialog(parent), __client(client)
{

    validator = new QIntValidator(1, 999, this);

    std::string msg = "How many action points do you want to buy ? ";
    std::string msg2 ="(1 AP = " + intToString(GOLD_FOR_ONE_AP) +" gold) ";
    label = new QLabel(tr(msg.c_str()));
    label2 = new QLabel(tr(msg2.c_str()));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);
    lineEdit->setValidator(validator);
    confirmationButton = new QPushButton(tr("Buy"));
    confirmationButton->setDefault(true);
    closeButton = new QPushButton(tr("Quit"));

    connect(lineEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(enableConfirmationButton(const QString &)));
    connect(confirmationButton, SIGNAL(clicked()),
            this, SLOT(confirmationClicked()));
    connect(closeButton, SIGNAL(clicked()),
            this, SLOT(close()));

    QVBoxLayout *topLeftLayout = new QVBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(label2);

    QHBoxLayout *rightLayout = new QHBoxLayout;
    rightLayout->addWidget(lineEdit);
    rightLayout->addWidget(confirmationButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(topLeftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    errorMessageDialog = new QErrorMessage(this);

    setWindowTitle(tr("Buy action points"));
    setFixedHeight(sizeHint().height());
    init();
}

void BuyAPDialog::init() {
    number="";
    label->setText(tr("Amount of action points you want to buy"));
    lineEdit->clear();
    lineEdit->setEchoMode(lineEdit->Normal);
    confirmationButton->setEnabled(false);
    setResult(Rejected);
}

int BuyAPDialog::getNumberOfAP() {return numberOfAP;}

void BuyAPDialog::confirmationClicked()
{
    if (lineEdit->hasAcceptableInput()) {
        QString text = lineEdit->text();
        number = text;
        numberOfAP = number.toInt();
        if (__client->buyActionPoints(numberOfAP)!=0){
            int result = __client->getPriceForAP();
            if (result<=0) {
                errorMessageDialog->showMessage(tr("Error : You don't have enough money."));
            }
            else {
                QMessageBox msgBox;
                QString msg = "You have bought "+number+" action points.";
                msgBox.setText(msg);
                msgBox.exec();
            }
        }
        reject();
    }
    else {
        errorMessageDialog->showMessage(
            tr("You must enter an integer value"));
        init();
    }
}

void BuyAPDialog::enableConfirmationButton(const QString &text) {
    confirmationButton->setEnabled(!text.isEmpty());
}
