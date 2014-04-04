#ifndef CLIENTMATCHHANDLER_HPP
#define CLIENTMATCHHANDLER_HPP
#include "Defines.hpp"
#include "SelectionDialog.hpp"
//#include "common/NetworkInterface.hpp"
#include "Client.hpp"
#include "PlayersDialog.hpp"
#define BAD_CONNECTION -1
#define NO_CHOICE -2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>

#include <QStringList>
#include <QWidget>
#include <QString>

int choosePartner(Client * client, QWidget *parent);
//~ int chooseTournament(Client * client, QWidget *parent);

std::vector<int> chooseTeamForMatch(Client * client, QWidget *parent);



#endif
