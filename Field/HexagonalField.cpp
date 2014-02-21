#include "HexagonalField.hpp"
#include <iostream>
#include <algorithm> //max()
#include <cstring>
#include <string>

HexagonalField::HexagonalField(){
    //-2 : hors de la grille hexagonale
    //-1 : place vide
    //>= 0 : objet
    for (int indexRow = 0; indexRow < MATRIX_SIZE; ++indexRow){
        for(int indexCol = 0; indexCol < std::max(0,((MATRIX_SIZE/2) - indexRow)); ++indexCol){
            matrix[indexRow][indexCol] = -2;
        }
        for(int indexCol = std::max(0,((MATRIX_SIZE/2) - indexRow)); indexCol < (MATRIX_SIZE - std::max(0, indexRow - (MATRIX_SIZE /2))); ++indexCol){
            matrix[indexRow][indexCol] = -1;
        }
        for(int indexCol = (MATRIX_SIZE - std::max(0, indexRow - (MATRIX_SIZE /2))); indexCol < MATRIX_SIZE; ++indexCol){
            matrix[indexRow][indexCol] = -2;
        }
    }
}

int HexagonalField::getOccupant(AxialCoordinates coord){
    return matrix[coord.getLineOnMatrix()][coord.getColOnMatrix()];
}

void HexagonalField::display() {
    char space = ' ';
    std::string toPrint;
    for (int indexRow = 0; indexRow < MATRIX_SIZE; ++indexRow){
        std::string decalage(abs(indexRow - (MATRIX_SIZE/2)) * 3, space);
        std::cout << decalage;
        for(int indexCol = std::max(0,((MATRIX_SIZE/2) - indexRow)); indexCol < (MATRIX_SIZE - std::max(0, indexRow - (MATRIX_SIZE /2))); ++indexCol){
            if (matrix[indexRow][indexCol]==-1) toPrint = "\u2B24";
            std::cout << toPrint << " " << std::string(4, space); //séparateur
        }
        std::cout << std::endl;


    }

    std::cout<< "\u2460" <<" "<<"\u2776"<<" "<<"\u2A02"<< " "<< "\u2B24" <<" "<< "\u2A00" <<" "<< "\u2A01" <<"  "<<" \uFF2F"<<std::endl;
    std::cout<<"  \u20DD"<<std::endl;
    std::cout<<"\u2460"<<std::endl;
}
    
