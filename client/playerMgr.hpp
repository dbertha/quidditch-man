#ifndef PLAYERMGR_HPP
#define PLAYERMGR_HPP
#include "Defines.hpp"
#include "selectionDialog.hpp"
#include "common/NetworkInterface.hpp"

#define BAD_CONNECTION -1
#define NO_CHOISE 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>

#include <QStringList>
#include <QWidget>

int choosePlayer(const int, QWidget *parent);

std::vector<int> chooseTeamForMatch(const int sockfd_, QWidget *parent);

#endif
