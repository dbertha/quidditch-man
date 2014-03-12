include (common.pri)
TEMPLATE = app
TARGET = clientGui
DEPENDPATH += . common


# Input
HEADERS += clientMatchHandler.hpp \
           loginDialog.hpp \
           mainGui.hpp \
           selectionDialog.hpp \
           ticker.hpp \
           BuildingsDialog.hpp \
           buildingsModel.hpp \
           playerMgr.hpp \

SOURCES += clientMatchHandler.cpp \
           loginDialog.cpp \
           main.cpp \
           mainGui.cpp \
           selectionDialog.cpp \
           ticker.cpp \
           BuildingsDialog.cpp \
           buildingsModel.cpp \
           playerMgr.cpp \

