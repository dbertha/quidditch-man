#ifndef MAINMENU_HPP
#define MAINMENU_HPP
#define MINHEIGHT 40
#define MAXWIDTH 160
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include "MainGui.hpp"
class MainGui;
class MainMenu : public QWidget {
    Q_OBJECT
public:
    explicit MainMenu(MainGui *parent = 0);

private:
    QPushButton *matchButton;
    QPushButton *tournamentButton;
    QPushButton *trainingMatchButton;
    QPushButton *playersButton;
    QPushButton *auctionButton;
    QPushButton *buildingsButton;
    QPushButton *buyAPButton;
    QPushButton *freeAPButton;
    QGridLayout *menuLayout;
    QGroupBox *groupBox;
    MainGui *parent_;
};

#endif
