#ifndef TOURNAMENTS_HPP
#define TOURNAMENTS_HPP




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>

#include <QStringList>
#include <QWidget>
#include <QMessageBox>
#include <QString>

#include "tournamentDialog.hpp"
#include "Client.hpp"
#include "../common/Defines.hpp"

int chooseTournament(Client *, const int, QWidget *parent);

#endif
