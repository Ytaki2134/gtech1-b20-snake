#include <time.h>
#include "playground.hpp"

Playground::Playground(int nbRows, int nbCols, Snake* snake){
    this->nbCols = nbCols;
    this->nbRows = nbRows;
    this->snake = snake;
    
    SpawnFruit();
}

Playground::~Playground(){
}

int Playground::GetNbCols(){
    return nbCols;
}

int Playground::GetNbRows(){
    return nbRows;
}

Fruit* Playground::GetFruit(){
    return presentFruit;
}

Snake* Playground::GetSnake(){
    return snake;
}

void Playground::SetFruit(Fruit* newFruit){
    presentFruit = newFruit;
}

//Creates new fruit with random attributes
void Playground::SpawnFruit(){
    srand(time(NULL));

    FruitEffect randomEffect;
    int randomEffectNumber = (rand()%10) + 1;

    //10% chance to get a special fruit
    if(randomEffectNumber == 10){
        randomEffect = static_cast<FruitEffect>((rand()% (COUNT-1)) + 1);
    }
    else{
        randomEffect = BONUS;
    }
    
    //Check if random coordinates doesn't collide with anything, if it does it tries again with new ones
    int randomCol = rand()%nbCols;
    int randomRow = rand()%nbRows;
    while(snake->occupiesTile(randomRow, randomCol))
    {
        randomCol = rand()%nbCols;
        randomRow = rand()%nbRows;
    }
    
    //If fruit coordinates doesn't collide with anything, we can make the fruit appear
    presentFruit = new Fruit(randomRow, randomCol, randomEffect);
}

