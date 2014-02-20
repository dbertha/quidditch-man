#include "Defines.hpp"
#ifndef COMMAPI_H
#define COMMAPI_H

#include <iostream>
#include <set>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//TODO : tout réunir dans le module NetworkBase
int buildConnection (const int);
//~ int sendMsg(const int, char *, const int);
//~ int receiveMsg(const int, char *, const int);
#endif
