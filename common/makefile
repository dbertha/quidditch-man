all : NetworkBaseTest.out Client.out Server.out

NetworkBaseTest.out : NetworkBase.o NetworkBaseTest.cpp Serialisator.cpp
	g++ -Wall -Wextra NetworkBase.o NetworkBaseTest.cpp Serialisator.cpp -o NetworkBaseTest.out

NetworkBase.o : NetworkBase.c NetworkBase.h
	gcc -c -Wall -Wextra NetworkBase.c

Client.out : NetworkBase.o SerialisationTestClient.c
	gcc -Wall -Wextra NetworkBase.o SerialisationTestClient.c -o Client.out

Server.out : NetworkBase.o SerialisationTestServer.c
	gcc -Wall -Wextra NetworkBase.o SerialisationTestServer.c -o Server.out
