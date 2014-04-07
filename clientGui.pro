include (common.pri)
TEMPLATE = app
TARGET = clientGui.out
DEPENDPATH += . client common
QT += widgets
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x000000
DEFINES += QT_NO_DEBUG_OUTPUT
# Input
HEADERS += \
           client/Client.hpp \
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
    client/AdminPage.hpp \
    client/TournamentsWidget.hpp \
    client/NotificationWidget.hpp \
    client/SelectPlayersWidget.hpp
           

SOURCES += \
           client/GUImain.cpp \
           client/Client.cpp \
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
    client/AdminPage.cpp \
    client/TournamentsWidget.cpp \
    client/NotificationWidget.cpp \
    client/SelectPlayersWidget.cpp

RESOURCES += \
    ressources.qrc

