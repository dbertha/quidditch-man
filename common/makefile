all : NetworkBaseTest.out

NetworkBaseTest.out : NetworkBase.o NetworkBaseTest.cpp
	g++ -Wall -Wpedantic -Wextra NetworkBase.o NetworkBaseTest.cpp -o NetworkBaseTest.out

NetworkBase.o : NetworkBase.c NetworkBase.h
	gcc -c -Wall -Wpedantic -Wextra NetworkBase.c
