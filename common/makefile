all : Client.out Server.out

#NetworkBaseTest.out : NetworkBase.o NetworkBaseTest.cpp
#	g++ -Wall -Wextra NetworkBase.o NetworkBaseTest.cpp -o NetworkBaseTest.out

NetworkBase.o : NetworkBase.c NetworkBase.h
	gcc -c -Wall -Wextra NetworkBase.c

Client.out : NetworkBase.o SerialisationTestClient.c NetworkInterface.hpp
	gcc -Wall -Wextra NetworkBase.o SerialisationTestClient.c -o Client.out

Server.out : NetworkBase.o SerialisationTestServer.c NetworkInterface.hpp
	gcc -Wall -Wextra NetworkBase.o SerialisationTestServer.c -o Server.out
