#ifndef POSITIONONFIELD_HPP
#define POSITIONONFIELD_HPP

#define MATRIX_SIZE 20

class PositionOnField {
    //Il serait plus cohérent de ne pas pouvoir modifier l'objet Position 
    //comme ça revient généralement à en modifier tous les attributs
    //il vaut mieux recréer une instance
private :
    const unsigned int __numOfMatrix;
    const unsigned int __row;
    const unsigned int __col;

    
public :
    PositionOnField(unsigned int numOfMatrix, unsigned int row, unsigned int col);
    unsigned int getNumOfMatrix() const;
    unsigned int getCol() const;
    unsigned int getRow() const;

    //~ inrow int setNumOfMatrix(int numOfMatrix); 
    //~ inrow int setcol(int col);
    //~ inrow int setrow(int row);
};



#endif
