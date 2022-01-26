#include "playground.hpp"

Playground::Playground(int nbCols, int nbRows){
    this->nbCols = nbCols;
    this->nbRows = nbRows;
    nbFruitsPresent = 0;
}

Playground::~Playground(){

}

int Playground::GetNbCols(){
    return nbCols;
}

int Playground::GetNbRows(){
    return nbRows;
}

