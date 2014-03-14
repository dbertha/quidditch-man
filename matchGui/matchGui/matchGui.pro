QMAKE_CXXFLAGS += -Wextra -std=c++11

SOURCES += \
    main.cpp \
    fenetre.cpp \
    hexagone.cpp \
    ManagedPlayer.cpp \
    PlayingPlayer.cpp \
    Broomstick.cpp \
    Player.cpp \
    HexagonalField.cpp

QT+=widgets

HEADERS += \
    fenetre.hpp \
    hexagone.hpp \
    Coordinates.hpp \
    Defines.hpp \
    ManagedPlayer.hpp \
    PlayingPlayer.hpp \
    Ball.hpp \
    Broomstick.hpp \
    Player.hpp \
    HexagonalField.hpp
