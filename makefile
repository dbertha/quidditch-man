COMMONDIR= common/
SERVERDIR= server/
CLIENTDIR= client/
COMMONHEADERS= NetworkInterface.hpp 
GCC = gcc -Wall -Wextra -std=c99
G++ = g++ -Wall -Wextra
NOLINKING = -c
DEBUGINFOS = -g
#COMMONCPP= 

all : ClientTest.out ServerTest.out Serveur.out 


#NetworkBaseTest.out : NetworkBase.o NetworkBaseTest.cpp
#	g++ -Wall -Wextra NetworkBase.o NetworkBaseTest.cpp -o NetworkBaseTest.out

ClientTest.out : NetworkBase.o ${CLIENTDIR}SerialisationTestClient.c ${COMMONDIR}${COMMONHEADERS}
	${GCC} ${DEBUGINFOS} NetworkBase.o ${CLIENTDIR}SerialisationTestClient.c -o ClientTest.out

ServerTest.out : NetworkBase.o ${SERVERDIR}SerialisationTestServer.c ${COMMONDIR}${COMMONHEADERS}
	${GCC} ${DEBUGINFOS} NetworkBase.o ${SERVERDIR}SerialisationTestServer.c -o ServerTest.out

NetworkBase.o : ${COMMONDIR}NetworkBase.c ${COMMONDIR}NetworkBase.h
	${GCC} ${NOLINKING} ${COMMONDIR}NetworkBase.c -o NetworkBase.o

Serveur.out : ${COMMONDIR}Defines.hpp ${SERVERDIR}Server.hpp ${SERVERDIR}User.hpp ${SERVERDIR}ServerMain.cpp ${SERVERDIR}Server.cpp ${SERVERDIR}User.cpp ${SERVERDIR}CommonMgr.cpp ${SERVERDIR}CommonMgr.hpp ${COMMONDIR}commAPI.cpp  ${COMMONDIR}commAPI.hpp
	${G++} ${DEBUGINFOS} ${SERVERDIR}ServerMain.cpp ${SERVERDIR}Server.cpp ${SERVERDIR}User.cpp ${SERVERDIR}CommonMgr.cpp ${COMMONDIR}commAPI.cpp -o Server.out

#Client.out : ${CLIENTDIR}client.c
#	${GCC} ${DEBUGINFOS} ${CLIENTDIR}client.c -o Client.out
