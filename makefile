COMMONDIR= common/
SERVERDIR= server/
CLIENTDIR= client/
COMMONHEADERS= NetworkInterface.hpp 
#COMMONCPP= 

all : Client.out Server.out


#NetworkBaseTest.out : NetworkBase.o NetworkBaseTest.cpp
#	g++ -Wall -Wextra NetworkBase.o NetworkBaseTest.cpp -o NetworkBaseTest.out

Client.out : NetworkBase.o ${CLIENTDIR}SerialisationTestClient.c ${COMMONDIR}${COMMONHEADERS}
	gcc -Wall -Wextra NetworkBase.o ${CLIENTDIR}SerialisationTestClient.c -o Client.out

Server.out : NetworkBase.o ${SERVERDIR}SerialisationTestServer.c ${COMMONDIR}${COMMONHEADERS}
	gcc -Wall -Wextra NetworkBase.o ${SERVERDIR}SerialisationTestServer.c -o Server.out

NetworkBase.o : ${COMMONDIR}NetworkBase.c ${COMMONDIR}NetworkBase.h
	gcc -c -Wall -Wextra ${COMMONDIR}NetworkBase.c -o NetworkBase.o


