#include "HexagonalField.hpp"
#include <iostream>
#include <algorithm> //max()
#include <cstring>
#include <iomanip> //setw
void HexagonalField::reset(){
    for (int indexRow = 0; indexRow < MATRIX_SIZE; ++indexRow){
        for(int indexCol = 0; indexCol < std::max(0,((MATRIX_SIZE/2) - indexRow)); ++indexCol){
            matrix[indexRow][indexCol] = NOT_ON_HEX_GRID;
        }
        for(int indexCol = std::max(0,((MATRIX_SIZE/2) - indexRow)); indexCol < (MATRIX_SIZE - std::max(0, indexRow - (MATRIX_SIZE /2))); ++indexCol){
            matrix[indexRow][indexCol] = FREE_SPACE;
        }
        for(int indexCol = (MATRIX_SIZE - std::max(0, indexRow - (MATRIX_SIZE /2))); indexCol < MATRIX_SIZE; ++indexCol){
            matrix[indexRow][indexCol] = NOT_ON_HEX_GRID;
        }
    }
}

HexagonalField::HexagonalField(){
    //-2 : hors de la grille hexagonale
    //-1 : place vide
    //>= 0 : objet
    reset();
    //ordre identique à la liste des joueurs
    unicodes.push_back(TEAM1_KEEPER_UNICODE);
    unicodes.push_back(TEAM1_SEEKER_UNICODE);
    unicodes.push_back(TEAM1_CHASER1_UNICODE);
    unicodes.push_back(TEAM1_CHASER2_UNICODE);
    unicodes.push_back(TEAM1_CHASER3_UNICODE);
    unicodes.push_back(TEAM1_BEATER1_UNICODE);
    unicodes.push_back(TEAM1_BEATER2_UNICODE);
    unicodes.push_back(TEAM2_KEEPER_UNICODE);
    unicodes.push_back(TEAM2_SEEKER_UNICODE);
    unicodes.push_back(TEAM2_CHASER1_UNICODE);
    unicodes.push_back(TEAM2_CHASER2_UNICODE);
    unicodes.push_back(TEAM2_CHASER3_UNICODE);
    unicodes.push_back(TEAM2_BEATER1_UNICODE);
    unicodes.push_back(TEAM2_BEATER2_UNICODE);
    unicodes.push_back(GOLDENSNITCH_UNICODE);
    unicodes.push_back(BLUDGER1_UNICODE);
    unicodes.push_back(BLUDGER2_UNICODE);
    unicodes.push_back(QUAFFLE_UNICODE);
    
}

int HexagonalField::getOccupant(AxialCoordinates coord) const{
    return matrix[coord.getLineOnMatrix()][coord.getColOnMatrix()];
}

bool HexagonalField::setOccupant(AxialCoordinates coord, objectIndex_t object){
    if(coord.isOnField()){
        matrix[coord.getLineOnMatrix()][coord.getColOnMatrix()] = object;
    }
    return coord.isOnField();
}

void HexagonalField::display() {
    //AxialCoordinates goal1Position(GOAL_SIDE1_DIAG, GOAL_SIDE1_LINE), goal2Position(GOAL_SIDE2_DIAG, GOAL_SIDE2_LINE);
    char space = ' ';
    std::string emptyUnicode = FREE_SPACE_UNICODE;
    for (int indexRow = 0; indexRow < MATRIX_SIZE; ++indexRow){
		std::string decalage;
		if(indexRow != (MATRIX_SIZE/2)) {
			decalage = std::string((abs(indexRow - (MATRIX_SIZE/2)) * 2.75), space);
		}
        std::cout << decalage;
        for(int indexCol = std::max(0,((MATRIX_SIZE/2) - indexRow)); indexCol < (MATRIX_SIZE - std::max(0, indexRow - (MATRIX_SIZE /2))); ++indexCol){
            if(matrix[indexRow][indexCol] < 0){
                if(isGoal(indexRow, indexCol)){ //affichage du goal
                    std::string goalUnicode = GOAL_UNICODE;
                    std::cout /*<< std::setw(2)*/ << goalUnicode << std::string(4, space);
                }
                else{
                    std::cout /*<< std::setw(2)*/ << emptyUnicode << std::string(4, space); //séparateur
                }
            }
            else{
                std::cout << /*std::setw(2) <<*/ unicodes[matrix[indexRow][indexCol]] << std::string(4, space); //séparateur
            }
        }
        std::cout << std::endl;
    }
}
    
void HexagonalField::display(AxialCoordinates selected, int distance){
    //AxialCoordinates goal1Position(GOAL_SIDE1_DIAG, GOAL_SIDE1_LINE), goal2Position(GOAL_SIDE2_DIAG, GOAL_SIDE2_LINE);
    std::string startColoring = "\033[31m"; //31 : rouge
    std::string stopColoring = "\033[0m"; // 0 : reset 
    std::string colorCenter = "\033[1;32m"; // 1 : gras, 32 : vert
    std::string emptyUnicode = FREE_SPACE_UNICODE;
    char space = ' ';
    for (int indexRow = 0; indexRow < MATRIX_SIZE; ++indexRow){
		std::string decalage;
        if(indexRow != (MATRIX_SIZE/2)) {
			decalage = std::string((abs(indexRow - (MATRIX_SIZE/2)) * 3) - 1, space);
		}
        std::cout << decalage;
        if((indexRow >= (selected.getLineOnMatrix() - distance)) and (indexRow <= (selected.getLineOnMatrix() + distance))){
            //lignes dont une partie doit être coloriée
            for(int indexCol = std::max(0,((MATRIX_SIZE/2) - indexRow)); indexCol < (MATRIX_SIZE - std::max(0, indexRow - (MATRIX_SIZE /2))); ++indexCol){
                if((indexCol == selected.getColOnMatrix() - distance + std::max(0, selected.getLineOnMatrix() - indexRow))
                //si colonne commence déjà plus loin que la colonne de départ de la coloration (cas aux extrémités)
                or ((indexCol == std::max(0,((MATRIX_SIZE/2) - indexRow))) 
                and (indexCol > selected.getColOnMatrix() - distance + std::max(0, selected.getLineOnMatrix() - indexRow)))){
                    //colonne du début de la coloration
                    std::cout << startColoring;
                }
                if((indexRow == selected.getLineOnMatrix()) and (indexCol == selected.getColOnMatrix())){
                    std::cout << colorCenter; //coloration particulière pour le centre
                }
                if(matrix[indexRow][indexCol] < 0){
                    if(isGoal(indexRow, indexCol)){ //affichage des buts
                        std::string goalUnicode = GOAL_UNICODE;
                        std::cout << /*std::setw(2) <<*/  goalUnicode << std::string(4, space);
                    }
                    else{
                        std::cout << /*std::setw(2) << */  emptyUnicode << std::string(4, space); //séparateur
                    }
                }
                else{
                    std::cout << /*std::setw(2) <<*/ unicodes[matrix[indexRow][indexCol]] << std::string(4, space); //séparateur
                }
                if((indexRow == selected.getLineOnMatrix()) and (indexCol == selected.getColOnMatrix())){
                    std::cout << stopColoring;
                    std::cout << startColoring; // une fois le centre passé, on reprend la coloration normale
                }
                if(indexCol == selected.getColOnMatrix() + distance - std::max(0, indexRow - selected.getLineOnMatrix())){
                    //colonne de fin de la coloration
                    std::cout << stopColoring;
                }
                
            }
            std::cout << stopColoring; //fin de la coloration si fin de ligne
        }else{
            for(int indexCol = std::max(0,((MATRIX_SIZE/2) - indexRow)); indexCol < (MATRIX_SIZE - std::max(0, indexRow - (MATRIX_SIZE /2))); ++indexCol){
                if(matrix[indexRow][indexCol] < 0){
                    if(isGoal(indexRow,indexCol)){ //affichage des buts
                        std::string goalUnicode = GOAL_UNICODE;
                        std::cout << /*std::setw(2) <<*/  goalUnicode << std::string(4, space);
                    }
                    else{
                        std::cout << /*std::setw(2) << */  emptyUnicode << std::string(4, space); //séparateur
                    }
                }
                else{
                    std::cout << /*std::setw(2) <<*/ unicodes[matrix[indexRow][indexCol]] << std::string(4, space); //séparateur
                }
            }
        }
        std::cout << std::endl;
    }
}


bool HexagonalField::isFree(AxialCoordinates toTest){
    return matrix[toTest.getLineOnMatrix()][toTest.getColOnMatrix()] == FREE_SPACE;
}

bool HexagonalField::isGoal(int rowOnMatrix, int colOnMatrix){
	return isGoal(AxialCoordinates(colOnMatrix - MATRIX_SIZE/2, rowOnMatrix - MATRIX_SIZE / 2));
}

bool HexagonalField::isGoal(AxialCoordinates toTest){
	return isTeam1Goal(toTest) or isTeam2Goal(toTest);
}

bool HexagonalField::isTeam1Goal(AxialCoordinates toTest){
	int diag = toTest.getDiagAxis();
	int line = toTest.getLineAxis();
	return (diag == GOAL1_SIDE1_DIAG and line == GOAL1_SIDE1_LINE) or 
		(diag == GOAL2_SIDE1_DIAG and line == GOAL2_SIDE1_LINE) or 
		(diag == GOAL3_SIDE1_DIAG and line == GOAL3_SIDE1_LINE);
}

bool HexagonalField::isTeam2Goal(AxialCoordinates toTest){
	int diag = toTest.getDiagAxis();
	int line = toTest.getLineAxis();
	return (diag == GOAL1_SIDE2_DIAG and line == GOAL1_SIDE2_LINE) or 
		(diag == GOAL2_SIDE2_DIAG and line == GOAL2_SIDE2_LINE) or 
		(diag == GOAL3_SIDE2_DIAG and line == GOAL3_SIDE2_LINE);
}
