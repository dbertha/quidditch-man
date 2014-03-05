#ifndef CLIENTMATCHHANDLER_HPP
#define CLIENTMATCHHANDLER_HPP
#include "Defines.hpp"
#include "selectionDialog.hpp"
#include "common/NetworkInterface.hpp"

#define BAD_CONNECTION 3
#define NO_PARTNER 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>

#include <QStringList>
#include <QWidget>

int choosePartner(const int, QWidget *parent);


#endif // CLIENTMATCHHANDLER_HPP
