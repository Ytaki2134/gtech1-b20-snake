#include "score.hpp"

Score::Score(void){
    number_eaten = 0;
    score = 0;
}

Score::~Score(void){

}

void Score::update_numb_eaten(int value){
    number_eaten += value;
    if(number_eaten < 0){
        number_eaten = 0;
    }
}

void Score::update_score(int value){
    score += value;
}

int Score::getScore(){
    return score;
}

int Score::getNumberEaten(){
    return number_eaten;
}
