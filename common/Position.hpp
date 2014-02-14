#ifndef Position_hpp
#define Position_hpp


class PlayingPlayer;
class Field;
class Ball;

class Position {

 public:

    int getNumOfMatrice();

    int getLine();

    int getColumn();

 public:
    int numOfMatrice;
    int line;
    int column;

#endif // Position_hpp
