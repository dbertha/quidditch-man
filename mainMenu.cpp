#include "mainMenu.hpp"

MainMenu::MainMenu(MainGui *parent) : QWidget(parent), parent_(parent) {
    matchButton = new QPushButton(tr("Friendly match"));
    matchButton->setMinimumHeight(MINHEIGHT);
    matchButton->setMaximumWidth(MAXWIDTH);
    tournamentButton = new QPushButton(tr("Tournaments"));
    tournamentButton->setMinimumHeight(MINHEIGHT);
    tournamentButton->setMaximumWidth(MAXWIDTH);
    playersButton = new QPushButton(tr("Your players"));
    playersButton->setMinimumHeight(MINHEIGHT);
    playersButton->setMaximumWidth(MAXWIDTH);
    auctionButton = new QPushButton(tr("Auctions"));
    auctionButton->setMinimumHeight(MINHEIGHT);
    auctionButton->setMaximumWidth(MAXWIDTH);
    buildingsButton = new QPushButton(tr("Your buildings"));
    buildingsButton->setMinimumHeight(MINHEIGHT);
    buildingsButton->setMaximumWidth(MAXWIDTH);
    APButton = new QPushButton(tr("Action points"));
    APButton->setMinimumHeight(MINHEIGHT);
    APButton->setMaximumWidth(MAXWIDTH);
    menuLayout = new QVBoxLayout;
    menuLayout->addWidget(matchButton);
    menuLayout->addWidget(tournamentButton);
    menuLayout->addWidget(playersButton);
    menuLayout->addWidget(auctionButton);
    menuLayout->addWidget(buildingsButton);
    menuLayout->addWidget(APButton);
    setLayout(menuLayout);
    connect(matchButton,SIGNAL(clicked()),parent_,SLOT(listMgrs()));
    connect(playersButton,SIGNAL(clicked()),parent_,SLOT(listPlayers()));
    connect(buildingsButton,SIGNAL(clicked()),parent_,SLOT(buildings()));
}
