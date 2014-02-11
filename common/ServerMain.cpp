#include "Server.h"
#include "Defines.h"

int main(int argc, char* argv()) {
	Server ourServer=Server(PORT);
	ourServer.run();
	return EXIT_SUCCESS;
}
