#include "HexagonalField.hpp"
#include <iostream>
#include <algorithm> //max()
#include <cstring>
#include <iomanip> //setw

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

int HexagonalField::getOccupant(AxialCoordinates coord) const{
    return matrix[coord.getLineOnMatrix()][coord.getColOnMatrix()];
}

bool HexagonalField::setOccupant(AxialCoordinates coord, objectIndex_t object){
    if(coord.isOnMatrix()){
        matrix[coord.getLineOnMatrix()][coord.getColOnMatrix()] = object;
    }
    return coord.isOnMatrix();
}

void HexagonalField::display() {
    char space = ' ';
    for (int indexRow = 0; indexRow < MATRIX_SIZE; ++indexRow){
        std::string decalage(abs(indexRow - (MATRIX_SIZE/2)) * 3, space);
        std::cout << decalage;
        for(int indexCol = std::max(0,((MATRIX_SIZE/2) - indexRow)); indexCol < (MATRIX_SIZE - std::max(0, indexRow - (MATRIX_SIZE /2))); ++indexCol){
            std::cout << std::setw(2) << matrix[indexRow][indexCol] << std::string(4, space); //séparateur
        }
        std::cout << std::endl;
    }
}
    
