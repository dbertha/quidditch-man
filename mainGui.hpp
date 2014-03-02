#ifndef MAINGUI_H
#define MAINGUI_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <QMainWindow>
#include "loginDialog.hpp"
class MainGui : public QMainWindow{
    Q_OBJECT
public:
        MainGui(int,QMainWindow *parent=0);
        ~ MainGui();
        void run();
private slots:
        void about();
        void quit();
        int login();
private:
        void createMenu();
        QAction *loginAction;
        QAction *exitAction;
        QAction *aboutAction;
        QMenu *fileMenu;
        QMenu *helpMenu;
        LoginDialog *dialog;
        int sockfd_;
        QMainWindow parent_;
};

#endif // MAINGUI_H
