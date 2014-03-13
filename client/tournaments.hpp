#ifndef TOURNAMENTS_HPP
#define TOURNAMENTS_HPP

#define BAD_CONNECTION -1
#define NO_CHOISE 0

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

int chooseTournament(Client *, const int, QWidget *parent);

#endif
