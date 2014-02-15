#ifndef GAME_H
#define GAME_H

class Server;
class Game {
public:
	Game(Server *);

private:
	Server* server_;
};

#endif
