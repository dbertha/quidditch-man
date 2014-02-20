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

Serveur.out : ${COMMONDIR}Defines.hpp ${SERVERDIR}Server.hpp ${SERVERDIR}User.hpp ${SERVERDIR}ServerMain.cpp ${SERVERDIR}Server.cpp ${SERVERDIR}User.cpp ${SERVERDIR}CommonMgr.cpp ${SERVERDIR}CommonMgr.hpp ${COMMONDIR}commAPI.cpp  ${COMMONDIR}commAPI.hpp ${SERVERDIR}Manager.hpp ${SERVERDIR}Manager.cpp ${SERVERDIR}Player.cpp ${SERVERDIR}ManagedPlayer.cpp ${SERVERDIR}Broomstick.cpp ${SERVERDIR}Saver.cpp ${SERVERDIR}TrainingCenter.cpp ${SERVERDIR}Hospital.cpp ${SERVERDIR}RecruitmentCenter.cpp ${SERVERDIR}FanShop.cpp ${SERVERDIR}Stadium.cpp ${SERVERDIR}ImprovementBuilding.cpp ${SERVERDIR}Building.cpp ${SERVERDIR}Calendar.cpp ${SERVERDIR}Player.hpp ${SERVERDIR}ManagedPlayer.hpp ${SERVERDIR}Broomstick.hpp ${SERVERDIR}Saver.hpp ${SERVERDIR}TrainingCenter.hpp ${SERVERDIR}Hospital.hpp ${SERVERDIR}RecruitmentCenter.hpp ${SERVERDIR}FanShop.hpp ${SERVERDIR}Stadium.hpp ${SERVERDIR}ImprovementBuilding.hpp ${SERVERDIR}Building.hpp ${SERVERDIR}Calendar.hpp
	${G++} ${DEBUGINFOS} ${SERVERDIR}ServerMain.cpp ${SERVERDIR}Server.cpp ${SERVERDIR}User.cpp ${SERVERDIR}CommonMgr.cpp ${COMMONDIR}commAPI.cpp ${SERVERDIR}Manager.cpp ${SERVERDIR}Player.cpp ${SERVERDIR}ManagedPlayer.cpp ${SERVERDIR}Broomstick.cpp ${SERVERDIR}Saver.cpp ${SERVERDIR}TrainingCenter.cpp ${SERVERDIR}Hospital.cpp ${SERVERDIR}RecruitmentCenter.cpp ${SERVERDIR}FanShop.cpp ${SERVERDIR}Stadium.cpp ${SERVERDIR}ImprovementBuilding.cpp ${SERVERDIR}Building.cpp ${SERVERDIR}Calendar.cpp -o Server.out

#Client.out : ${CLIENTDIR}client.c
#	${GCC} ${DEBUGINFOS} ${CLIENTDIR}client.c -o Client.out
