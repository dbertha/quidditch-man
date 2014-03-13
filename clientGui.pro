include (common.pri)
TEMPLATE = app
TARGET = clientGui.out
DEPENDPATH += . client common


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

