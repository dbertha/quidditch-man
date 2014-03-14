#ifndef PLAYERMGR_HPP
#define PLAYERMGR_HPP
#include "Defines.hpp"
#include "selectionDialog.hpp"
#include "Client.hpp"
#include "playersDialog.hpp"
//#include "common/NetworkInterface.hpp"


#define BAD_CONNECTION -1
#define NO_CHOICE -2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>

#include <QStringList>
#include <QWidget>

int choosePlayer(Client * client, QWidget *parent);


#endif