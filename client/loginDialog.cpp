#include "loginDialog.hpp"

LoginDialog::LoginDialog(Client * client, QWidget *parent): QDialog(parent), __client(client)
{
    label = new QLabel(tr("User Name"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);
    QRegExp regExp("[A-Za-z0-9]{5,30}");
    lineEdit->setValidator(new QRegExpValidator(regExp,this));
    newUserCheckBox = new QCheckBox(tr("New User"));

    loginButton = new QPushButton(tr("Next"));
    loginButton->setDefault(true);
    closeButton = new QPushButton(tr("Quit"));

    connect(lineEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(enableloginButton(const QString &)));
    connect(loginButton, SIGNAL(clicked()),
            this, SLOT(loginClicked()));
    connect(closeButton, SIGNAL(clicked()),
            this, SLOT(close()));

    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(lineEdit);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addWidget(newUserCheckBox);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(loginButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    errorMessageDialog = new QErrorMessage(this);

    setWindowTitle(tr("login"));
    setFixedHeight(sizeHint().height());
    init();
}

void LoginDialog::init() {
    userName=" ";
    password1=" ";
    label->setText(tr("User Name"));
    lineEdit->clear();
    lineEdit->setEchoMode(lineEdit->Normal);
    loginButton->setEnabled(false);
    setResult(Rejected);
}

QString LoginDialog::getName() {return userName;}
int LoginDialog::getRole() {return role;}

void LoginDialog::loginClicked()
{
    if (lineEdit->hasAcceptableInput()) {
        QString text = lineEdit->text();
        if (userName==" ") {//le mgr a introduit son nom
            userName=text;
            std::cout<<"user "<<userName.toStdString()<<std::endl;
            label->setText(tr("Enter password"));
            lineEdit->clear();
            lineEdit->setEchoMode(lineEdit->Password);
            loginButton->setEnabled(false);
        }
        else if (password1==" ") {//le mgr a introduit son pswd
                    password1=text;
                    lineEdit->clear();
                    std::cout<<"pswd "<<password1.toStdString()<<std::endl;
                    if (newUserCheckBox->isChecked()) {//le mgr s'enregistre
                        label->setText(tr("Enter password again"));
                        loginButton->setEnabled(false);
                    }
                    else {//on tente le login
                        strncpy(username,userName.toAscii().data(),USERNAME_LENGTH);
                        strncpy(password,password1.toAscii().data(),PASSWORD_LENGTH);
                        if(__client->sendLoginToServer(username,password)!=0) {
                            role=__client->getConfirmation();
                             if (role) accept();
                             else {
                                 errorMessageDialog->showMessage(
                                         tr("Name or password invalid."));
                                 init();
                             }
                        }
                    }
            }
            else //vérifions si les 2 mots de passe sont identiques
                if (password1==text) {//on tente l'enregistrement
                    strncpy(username,userName.toAscii().data(),USERNAME_LENGTH);
                    strncpy(password,password1.toAscii().data(),PASSWORD_LENGTH);
                    if(__client->sendNewManagerToServer(username,password)!=0)
                        if (__client->getConfirmation() == NORMAL_LOGIN) accept();
                        else {
                            errorMessageDialog->showMessage(
                                tr("User name already registred."));
                            init();
                        }
                }
                else {
                    std::cout<<"pswd "<<text.toStdString()<<std::endl;
                    errorMessageDialog->showMessage(
                            tr("The passwords are not the same."));
                    password1=" ";
                    label->setText(tr("Enter password"));
                    lineEdit->clear();
                    loginButton->setEnabled(false);
                }
    }
    else {
        errorMessageDialog->showMessage(
            tr("Name and password may only hold 5 to 30 characters and digits"));
        init();
    }
}

void LoginDialog::enableloginButton(const QString &text) {
    loginButton->setEnabled(!text.isEmpty());
}
