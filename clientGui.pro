include (common.pri)
TEMPLATE = app
TARGET = clientGui.out
DEPENDPATH += . client common
QT += widgets
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x000000

# Input
HEADERS += client/ClientMatchHandler.hpp \
           client/LoginDialog.hpp \
           client/MainGui.hpp \
           client/SelectionDialog.hpp \
           client/Ticker.hpp \
           client/BuildingsDialog.hpp \
           client/BuildingsModel.hpp \
           client/PlayerMgr.hpp \
           client/Client.hpp \
           client/MainMenu.hpp \
           client/PlayersDialog.hpp \
           client/TournamentDialog.hpp \
           client/Tournaments.hpp \
           client/BuyAPDialog.hpp \
           client/FreeAPDialog.hpp \
           client/AuctionMgr.hpp \
           client/AuctionsDialog.hpp \
           client/AuctionWindow.hpp \
           client/MatchWindow.hpp \
           client/HexagonalCase.hpp \ 
    client/MainWindow.hpp \
    client/ConnexionPage.hpp \
    client/LoginPage.hpp \
    client/MainPage.hpp \
    client/InfosWidget.hpp \
    client/StadiumPage.hpp \
    client/OfficePage.hpp \
    client/DomainPage.hpp \
    client/BuildingWidget.hpp \
    client/BuyAPWidget.hpp \
    client/GainAPWidget.hpp \
    client/PlayerWidget.hpp \
    client/ListPlayersWidget.hpp \
    client/ManagePlayerWidget.hpp \
    client/ListAuctionsWidget.hpp \
    client/AuctionWidget.hpp \
    client/AdminPage.hpp
           

SOURCES += client/ClientMatchHandler.cpp \
           client/LoginDialog.cpp \
           client/GUImain.cpp \
           client/MainGui.cpp \
           client/SelectionDialog.cpp \
           client/Ticker.cpp \
           client/BuildingsDialog.cpp \
           client/BuildingsModel.cpp \
           client/PlayerMgr.cpp \
           client/Client.cpp \
           client/MainMenu.cpp \
           client/PlayersDialog.cpp \
           client/TournamentDialog.cpp \
           client/Tournaments.cpp \
           client/BuyAPDialog.cpp \
           client/FreeAPDialog.cpp \
           client/AuctionMgr.cpp \
           client/AuctionsDialog.cpp \
           client/AuctionWindow.cpp \
           client/MatchWindow.cpp \
           client/HexagonalCase.cpp \ 
    client/MainWindow.cpp \
    client/ConnexionPage.cpp \
    client/LoginPage.cpp \
    client/MainPage.cpp \
    client/InfosWidget.cpp \
    client/StadiumPage.cpp \
    client/OfficePage.cpp \
    client/DomainPage.cpp \
    client/BuildingWidget.cpp \
    client/BuyAPWidget.cpp \
    client/GainAPWidget.cpp \
    client/PlayerWidget.cpp \
    client/ListPlayersWidget.cpp \
    client/ManagePlayerWidget.cpp \
    client/ListAuctionsWidget.cpp \
    client/AuctionWidget.cpp \
    client/AdminPage.cpp

RESOURCES += \
    ressources.qrc

