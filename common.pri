#dépendances communes

CONFIG += debug
DEPENDPATH += . common
INCLUDEPATH += . common
QMAKE_CXXFLAGS += -Wextra


# Input
HEADERS += common/Coordinates.hpp \
           common/Defines.hpp \
           common/HexagonalField.hpp \
           common/NetworkBase.hpp \
           
SOURCES += common/HexagonalField.cpp \
           common/NetworkBase.cpp
