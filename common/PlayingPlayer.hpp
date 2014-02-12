#ifndef PlayingPlayer_hpp
#define PlayingPlayer_hpp

#include "Field.hpp"
#include "Match.hpp"
#include "Player.hpp"
#include "Position.hpp"

class PlayingPlayer : public Player {

 public:

 	Field field;
    Position position;

    void getMaxDistance();
    void avoidBludger();
    void move();

 private:

    void handleDamage();


};

#endif // PlayingPlayer_hpp
