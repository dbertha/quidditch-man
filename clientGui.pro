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

SOURCES += clientMatchHandler.cpp \
           loginDialog.cpp \
           main.cpp \
           mainGui.cpp \
           selectionDialog.cpp \
           ticker.cpp \

