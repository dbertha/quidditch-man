include (common.pri)
TEMPLATE = app
TARGET = clientGui.out
DEPENDPATH += . client common
QT += widgets
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x000000

# Input
HEADERS += client/clientMatchHandler.hpp \
           client/loginDialog.hpp \
           client/mainGui.hpp \
           client/selectionDialog.hpp \
           client/ticker.hpp \
           client/buildingsDialog.hpp \
           client/buildingsModel.hpp \
           client/playerMgr.hpp \
           client/Client.hpp \
           client/mainMenu.hpp \
           client/playersDialog.hpp \
           client/tournamentDialog.hpp \
           client/tournaments.hpp \
           client/buyAPDialog.hpp \
           client/freeAPDialog.hpp \
           client/auctionMgr.hpp \
           client/AuctionsDialog.hpp \
           client/auctionWindow.hpp \
           client/MatchWindow.hpp \
           client/HexagonalCase.hpp 
           

SOURCES += client/clientMatchHandler.cpp \
           client/loginDialog.cpp \
           client/GUImain.cpp \
           client/mainGui.cpp \
           client/selectionDialog.cpp \
           client/ticker.cpp \
           client/buildingsDialog.cpp \
           client/buildingsModel.cpp \
           client/playerMgr.cpp \
           client/Client.cpp \
           client/mainMenu.cpp \
           client/playersDialog.cpp \
           client/tournamentDialog.cpp \
           client/tournaments.cpp \
           client/buyAPDialog.cpp \
           client/freeAPDialog.cpp \
           client/auctionMgr.cpp \
           client/AuctionsDialog.cpp \
           client/auctionWindow.cpp \
           client/MatchWindow.cpp \
           client/HexagonalCase.cpp

