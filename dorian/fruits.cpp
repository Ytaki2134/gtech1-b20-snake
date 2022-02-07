#include "fruits.hpp"

Fruit::Fruit(int row, int col, FruitEffect effect){
    this->col = col;
    this->row = row;
    this->effect = effect;
}

Fruit::~Fruit(){

}

int Fruit::GetCol(){
    return col;
}

int Fruit::GetRow(){
    return row;
}

FruitEffect Fruit::GetEffect(){
    return effect;
}
