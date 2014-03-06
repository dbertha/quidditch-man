include (common.pri)

TEMPLATE = app
TARGET = client.out
DEPENDPATH += . client

HEADERS += client/Client.hpp
SOURCES += client/Client.cpp client/ClientMain.cpp
