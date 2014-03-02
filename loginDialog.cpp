#include <QtGui>

#include "loginDialog.hpp"

LoginDialog::LoginDialog(QWidget *parent): QDialog(parent)
{   userName=" ";
    password1=" ";
    password2=" ";
    label = new QLabel(tr("User Name"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);
    newUserCheckBox = new QCheckBox(tr("New User"));

    loginButton = new QPushButton(tr("Next"));
    loginButton->setDefault(true);
    loginButton->setEnabled(false);
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

    setWindowTitle(tr("login"));
    setFixedHeight(sizeHint().height());
}

void LoginDialog::loginClicked()
{
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
                    lineEdit->clear();

                }
            }
        else //vérifions si les 2 mots de passe sont identiques
            if (password1==password2) {//on tente l'enregistrement
                    lineEdit->setEchoMode(lineEdit->Normal);

            }
            else {
                std::cout<<"pswd "<<password2.toStdString()<<std::endl;
                errorMessageDialog = new QErrorMessage(this);
                errorMessageDialog->showMessage(
                        tr("The passwords are not the same"));
                password1=" ";
                label->setText(tr("Enter password"));
                lineEdit->clear();
                loginButton->setEnabled(false);
            }
}

void LoginDialog::enableloginButton(const QString &text)
{
    loginButton->setEnabled(!text.isEmpty());
}
