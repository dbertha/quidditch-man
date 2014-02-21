#ifndef PlayingPlayer_hpp
#define PlayingPlayer_hpp

//#include "Field.hpp"
//#include "Match.hpp"
#include "Player.hpp"
#include "ManagedPlayer.hpp"
//#include "Position.hpp"

class PlayingPlayer : public Player {

 	//Field field;
    //Position position;

 public:

 	PlayingPlayer(ManagedPlayer& player);

    void getMaxDistance();
    void avoidBludger();
    void move();

 private:

    void handleDamage();


};

#endif // PlayingPlayer_hpp
