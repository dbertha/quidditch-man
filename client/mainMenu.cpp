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
    buyAPButton = new QPushButton(tr("Buy action points"));
    buyAPButton->setMinimumHeight(MINHEIGHT);
    buyAPButton->setMaximumWidth(MAXWIDTH);
    freeAPButton = new QPushButton(tr("Get free action points"));
    freeAPButton->setMinimumHeight(MINHEIGHT);
    freeAPButton->setMaximumWidth(MAXWIDTH);
    menuLayout = new QVBoxLayout;
    menuLayout->addWidget(matchButton);
    menuLayout->addWidget(tournamentButton);
    menuLayout->addWidget(playersButton);
    menuLayout->addWidget(auctionButton);
    menuLayout->addWidget(buildingsButton);
    menuLayout->addWidget(buyAPButton);
    menuLayout->addWidget(freeAPButton);
    setLayout(menuLayout);
    connect(matchButton,SIGNAL(clicked()),parent_,SLOT(listMgrs()));
    connect(playersButton,SIGNAL(clicked()),parent_,SLOT(listPlayers()));
    connect(buildingsButton,SIGNAL(clicked()),parent_,SLOT(buildings()));
    connect(tournamentButton,SIGNAL(clicked()),parent_,SLOT(tournaments()));
    connect(buyAPButton,SIGNAL(clicked()),parent_,SLOT(buyAPMenu()));
    connect(freeAPButton,SIGNAL(clicked()),parent_,SLOT(freeAPMenu()));
    connect(auctionButton,SIGNAL(clicked()),parent_,SLOT(auctionsMenu()));
}
