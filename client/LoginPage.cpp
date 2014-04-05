#include "LoginPage.hpp"

#include <QtGui>

LoginPage::LoginPage(Client *client, MainWindow* parent,bool registration) : _client(client),_parent(parent),_registration(registration) {
	setStyleSheet("background-image: url(Pictures/connexionPage.jpg)");
	_errorLabel = new QLabel();
    _errorLabel->setStyleSheet("background-color: rgba(255, 255, 255, 40);color: red");
    _errorLabel->setVisible(false);
    _loginZone = new QLineEdit();
    _loginZone->setPlaceholderText(tr("Username"));
    _loginZone->setAlignment(Qt::AlignHCenter);

    _passwordZone = new QLineEdit();
    _passwordZone->setPlaceholderText(tr("Password"));
    _passwordZone->setEchoMode(QLineEdit::Password);
    _passwordZone->setAlignment(Qt::AlignHCenter);


    QFontMetrics metrics(QApplication::font());
    _loginZone->setMinimumWidth(200);
    _passwordZone->setMinimumWidth(200);

    QRegExp regExp("[A-Za-z0-9]{5,30}");
    _loginZone->setValidator(new QRegExpValidator(regExp,this));
    _passwordZone->setValidator(new QRegExpValidator(regExp,this));


    if (registration){
        _passwordZone2 = new QLineEdit();
        _passwordZone2->setPlaceholderText(tr("Confirm password"));
        _passwordZone2->setMinimumWidth(200);
        _passwordZone2->setEchoMode(QLineEdit::Password);
        _passwordZone2->setAlignment(Qt::AlignHCenter);
        _passwordZone2->setValidator(new QRegExpValidator(regExp,this));
    }

    _confirmButton = new QPushButton(tr("CONFIRM"));    
    _confirmButton->setMinimumWidth(200);
    _confirmButton->setMinimumHeight(30);
    _confirmButton->setMaximumWidth(200);
    _confirmButton->setMaximumHeight(30);
    _cancelButton = new QPushButton(tr("CANCEL"));
    _cancelButton->setMinimumWidth(200);
    _cancelButton->setMinimumHeight(30);
    _cancelButton->setMaximumWidth(200);
    _cancelButton->setMaximumHeight(30);
    QColor col(209,173,77);
    if(col.isValid()) {
        QString qss = QString("background-color: %1;color: rgb(255, 255, 255)").arg(col.name());
        _confirmButton->setStyleSheet(qss);
        _cancelButton->setStyleSheet(qss);
    }

	QWidget *panel = new QWidget(this);
	 
	QGridLayout *grid = new QGridLayout(this);
	panel->setLayout(grid);
	
    grid->addWidget(_errorLabel,8,2,Qt::AlignCenter);
	grid->addWidget(_loginZone, 9,2, Qt::AlignCenter );
	grid->addWidget(_passwordZone, 10,2, Qt::AlignCenter );
    if (registration) {
        grid->addWidget(_passwordZone2, 11,2, Qt::AlignCenter );
        grid->addWidget(_confirmButton, 12,2, Qt::AlignCenter );
        grid->addWidget(_cancelButton, 13,2, Qt::AlignCenter );
    }
	else {  
        grid->addWidget(_confirmButton, 11,2, Qt::AlignCenter );
        grid->addWidget(_cancelButton, 12,2, Qt::AlignCenter );
    }

	grid->setVerticalSpacing(3);
	grid->setHorizontalSpacing(0);
			//connect(navButton, SIGNAL(clicked()), mapper, SLOT(map()));

	for (int i=0;i<5;++i){
        grid->setColumnMinimumWidth(i,150);
        if (i==2) grid->setColumnMinimumWidth(i,200);
	}
	for (int i=0;i<19;++i){
		grid->setRowMinimumHeight(i,30);
	}
/*
    QVBoxLayout *_mainLayout = new QVBoxLayout;
    _mainLayout->addWidget(_registerButton);
    _mainLayout->addWidget(_loginButton);
    _mainLayout->addWidget(_closeButton);
    setLayout(_mainLayout);
*/
    setWindowTitle(tr("QUIDDITH MANAGER"));
    setFixedSize(800,640);

    connect(_confirmButton,SIGNAL(clicked()),this,SLOT(confirm()));
    connect(_cancelButton,SIGNAL(clicked()),this,SLOT(cancel()));
    //exec();
}

void LoginPage::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void LoginPage::confirm(){
    if (_loginZone->hasAcceptableInput()) {
        if (_passwordZone->hasAcceptableInput()){
            QString username,password;
            username=_loginZone->text();
            password=_passwordZone->text();
            if (!_registration){
                strncpy(_username,username.toAscii().data(),USERNAME_LENGTH);
                strncpy(_password,password.toAscii().data(),PASSWORD_LENGTH);
                if(_client->sendLoginToServer(_username,_password)!=0) {
                    int role=_client->getConfirmation();
                    if (role) _parent->connexion(role);
                    else makeErrorLabel("Wrong login/password.");
                }
            }
            else if (_registration && _passwordZone2->hasAcceptableInput()){
                QString password2 = _passwordZone2->text();
                if (password==password2){
                    strncpy(_username,username.toAscii().data(),USERNAME_LENGTH);
                    strncpy(_password,password.toAscii().data(),PASSWORD_LENGTH);
                    if(_client->sendNewManagerToServer(_username,_password)!=0){
                        int role=_client->getConfirmation();
                        if (role == NORMAL_LOGIN) _parent->connexion(role);
                        else makeErrorLabel("Login already taken.");
                    }
                }
                else makeErrorLabel("Passwords don't match.");
            }
            else makeErrorLabel("Invalid format (5-30 chars/digits).");
        }
        else makeErrorLabel("Invalid format (5-30 chars/digits).");
    }
    else makeErrorLabel("Invalid format (5-30 chars/digits).");
}

void LoginPage::cancel(){
    _parent->cancel();
}

void LoginPage::makeErrorLabel(std::string text){
    _errorLabel->setText(tr(text.c_str()));
    _errorLabel->setVisible(true);
    update();
}