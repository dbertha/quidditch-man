#include "Server.hpp"
#include "../common/Defines.hpp"

int main(int argc, char* argv[]) {
	Server ourServer=Server(PORT);
	ourServer.run();
	return EXIT_SUCCESS;
}
