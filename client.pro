include (common.pri)

TEMPLATE = app
TARGET = client.out
DEPENDPATH += . client

HEADERS += client/Client.hpp \
           client/TerminalGame.hpp
SOURCES += client/Client.cpp client/ClientMain.cpp \
           client/TerminalGame.cpp

OTHER_FILES += \
    client/Pictures/quidditch.jpg

