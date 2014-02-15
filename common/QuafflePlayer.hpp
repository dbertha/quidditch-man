#ifndef QuafflePlayer_hpp
#define QuafflePlayer_hpp

#include "PlayingPlayer.hp"

class Quaffle;

class QuafflePlayer : public PlayingPlayer {

 public:

    void passQuaffle();
    void catchQuaffle();
    void interceptQuaffle();

};

#endif //QuafflePlayer_hpp
