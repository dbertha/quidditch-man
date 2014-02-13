#ifndef POSITIONONFIELD_HPP
#define POSITIONONFIELD_HPP

#define MATRIX_SIZE 20

class PositionOnField {
    //Il serait plus cohérent de ne pas pouvoir modifier l'objet Position 
    //comme ça revient généralement à en modifier tous les attributs
    //il vaut mieux recréer une instance
private :
    const unsigned int __numOfMatrix;
    const unsigned int __line;
    const unsigned int __row;

    
public :
    PositionOnField(unsigned int numOfMatrix, unsigned int line, unsigned int row);
    inline unsigned int getNumOfMatrix() const;
    inline unsigned int getRow() const;
    inline unsigned int getLine() const;

    //~ inline int setNumOfMatrix(int numOfMatrix); 
    //~ inline int setRow(int row);
    //~ inline int setLine(int line);
};

#endif
