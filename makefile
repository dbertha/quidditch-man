COMMONDIR= common/
SERVERDIR= server/
CLIENTDIR= client/
COMMONHEADERS= NetworkInterface.hpp  
GCC = gcc -Wall -Wextra -std=c99
G++ = g++ -pthread 
NOLINKING = -c
DEBUGINFOS = -g
#COMMONCPP= 

all :   Client.out Serveur.out

#Match.out : ${SERVERDIR}testMatch.cpp ${SERVERDIR}MatchesHandler.cpp ${SERVERDIR}MatchesHandler.hpp ${SERVERDIR}Match.cpp ${SERVERDIR}Match.hpp ${SERVERDIR}PlayingPlayer.cpp ${SERVERDIR}PlayingPlayer.hpp ${COMMONDIR}HexagonalField.cpp ${COMMONDIR}HexagonalField.hpp ${COMMONDIR}Coordinates.hpp ${SERVERDIR}ManagedPlayer.cpp ${SERVERDIR}ManagedPlayer.hpp ${SERVERDIR}Player.cpp ${SERVERDIR}Player.hpp ${SERVERDIR}Broomstick.cpp ${SERVERDIR}Broomstick.hpp
#	${G++} ${DEBUGINFOS}  ${SERVERDIR}testMatch.cpp ${SERVERDIR}MatchesHandler.cpp ${SERVERDIR}Match.cpp ${SERVERDIR}PlayingPlayer.cpp ${COMMONDIR}HexagonalField.cpp ${SERVERDIR}ManagedPlayer.cpp ${SERVERDIR}Player.cpp ${SERVERDIR}Broomstick.cpp -o Match.out


#NetworkBaseTest.out : NetworkBase.o NetworkBaseTest.cpp
#	g++ -Wall -Wextra NetworkBase.o NetworkBaseTest.cpp -o NetworkBaseTest.out

#ClientTest.out : NetworkBase.o ${CLIENTDIR}SerialisationTestClient.c ${COMMONDIR}${COMMONHEADERS}
#	${G++} ${DEBUGINFOS} NetworkBase.o ${CLIENTDIR}SerialisationTestClient.c -o ClientTest.out

# ServerTest.out : NetworkBase.o ${SERVERDIR}SerialisationTestServer.c ${COMMONDIR}${COMMONHEADERS}
# 	${GCC} ${DEBUGINFOS} NetworkBase.o ${SERVERDIR}SerialisationTestServer.c -o ServerTest.out

NetworkBase.o : ${COMMONDIR}NetworkBase.c ${COMMONDIR}NetworkBase.h
	${G++} ${NOLINKING} ${COMMONDIR}NetworkBase.c -o NetworkBase.o

Serveur.out : NetworkBase.o ${SERVERDIR}MatchesHandler.cpp ${SERVERDIR}MatchesHandler.hpp ${SERVERDIR}Match.cpp ${SERVERDIR}Match.hpp ${SERVERDIR}PlayingPlayer.cpp ${SERVERDIR}PlayingPlayer.hpp ${COMMONDIR}HexagonalField.cpp ${COMMONDIR}HexagonalField.hpp ${COMMONDIR}Coordinates.hpp ${COMMONDIR}Defines.hpp ${SERVERDIR}Server.hpp ${SERVERDIR}User.hpp ${SERVERDIR}ServerMain.cpp ${SERVERDIR}Server.cpp ${SERVERDIR}User.cpp  ${COMMONDIR}commAPI.cpp  ${COMMONDIR}commAPI.hpp ${SERVERDIR}Manager.hpp ${SERVERDIR}Manager.cpp ${SERVERDIR}Player.cpp ${SERVERDIR}ManagedPlayer.cpp ${SERVERDIR}Broomstick.cpp ${SERVERDIR}Saver.cpp ${SERVERDIR}TrainingCenter.cpp ${SERVERDIR}Hospital.cpp ${SERVERDIR}RecruitmentCenter.cpp ${SERVERDIR}FanShop.cpp ${SERVERDIR}Stadium.cpp ${SERVERDIR}ImprovementBuilding.cpp ${SERVERDIR}Building.cpp ${SERVERDIR}Calendar.cpp ${SERVERDIR}Player.hpp ${SERVERDIR}ManagedPlayer.hpp ${SERVERDIR}Broomstick.hpp ${SERVERDIR}Saver.hpp ${SERVERDIR}TrainingCenter.hpp ${SERVERDIR}Hospital.hpp ${SERVERDIR}RecruitmentCenter.hpp ${SERVERDIR}FanShop.hpp ${SERVERDIR}Stadium.hpp ${SERVERDIR}ImprovementBuilding.hpp ${SERVERDIR}Building.hpp ${SERVERDIR}Calendar.hpp ${SERVERDIR}Auction.hpp ${SERVERDIR}Auction.cpp
	${G++} ${DEBUGINFOS} NetworkBase.o ${SERVERDIR}Match.cpp ${SERVERDIR}MatchesHandler.cpp ${SERVERDIR}PlayingPlayer.cpp ${COMMONDIR}HexagonalField.cpp ${SERVERDIR}ServerMain.cpp ${SERVERDIR}Server.cpp ${SERVERDIR}User.cpp ${COMMONDIR}commAPI.cpp ${SERVERDIR}Manager.cpp ${SERVERDIR}Player.cpp ${SERVERDIR}ManagedPlayer.cpp ${SERVERDIR}Broomstick.cpp ${SERVERDIR}Saver.cpp ${SERVERDIR}TrainingCenter.cpp ${SERVERDIR}Hospital.cpp ${SERVERDIR}RecruitmentCenter.cpp ${SERVERDIR}FanShop.cpp ${SERVERDIR}Stadium.cpp ${SERVERDIR}ImprovementBuilding.cpp ${SERVERDIR}Building.cpp ${SERVERDIR}Calendar.cpp ${SERVERDIR}Auction.cpp -o Server.out

Client.out : NetworkBase.o  ${CLIENTDIR}Client.cpp ${COMMONDIR}${COMMONHEADERS} ${COMMONDIR}HexagonalField.cpp ${COMMONDIR}HexagonalField.hpp ${COMMONDIR}Coordinates.hpp
	${G++} ${DEBUGINFOS} NetworkBase.o ${COMMONDIR}HexagonalField.cpp  ${CLIENTDIR}Client.cpp -o Client.out
