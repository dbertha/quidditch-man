#ifndef AUCTIONMGR_HPP
#define AUCTIONMGR_HPP
#include "../common/Defines.hpp"
#include "SelectionDialog.hpp"
#include "Client.hpp"
#include "AuctionsDialog.hpp"
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

int chooseAuction(Client * client, QWidget *parent);


#endif
