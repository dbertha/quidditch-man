#dépendances communes

CONFIG += debug
DEPENDPATH += . common
INCLUDEPATH += . common
QMAKE_CXXFLAGS += -Wextra


# Input
HEADERS += common/commAPI.hpp \
           common/Coordinates.hpp \
           common/Defines.hpp \
           common/HexagonalField.hpp \
           common/NetworkBase.hpp \
           common/NetworkInterface.hpp 
           
SOURCES += common/commAPI.cpp \
           common/HexagonalField.cpp \
           common/NetworkBase.cpp
