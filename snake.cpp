#include <algorithm>

#include "snake.hpp"
#include "MainSDLWindow.hpp"
#include "playground.hpp"

//Returns true boolean if direction is a combination (example Up-Left) and false otherwise
bool isCombinedDirection(Direction direction){
    return direction != RIGHT && direction != LEFT && direction != UP && direction != DOWN;
}

//Returns a boolean representing the fact that two directions are on the same axis
bool onSameAxis(Direction firstDirection, Direction secondDirection){
    return (firstDirection == RIGHT && secondDirection == LEFT) 
           || (firstDirection == LEFT && secondDirection == RIGHT)
           || (firstDirection == UP && secondDirection == DOWN)
           || (firstDirection == DOWN && secondDirection == UP);
}

/*Combines two direction to create a new direction such as firstDirection -> secondDirection
This combined direction is used to display angle turn textures for snake sprite*/
Direction CombinedDirection(Direction firstDirection, Direction secondDirection){
    if(firstDirection == secondDirection){
        return firstDirection;
    }

    switch(firstDirection){
        case RIGHT:
            switch(secondDirection){
                case UP:
                    return RIGHTUP;
                
                case DOWN:
                    return RIGHTDOWN;
            }
            break;
        
        case LEFT:
            switch(secondDirection){
                case UP:
                    return LEFTUP;
                
                case DOWN:
                    return LEFTDOWN;
            }
            break;
        
        case UP:
            switch(secondDirection){
                case RIGHT:
                    return UPRIGHT;
                
                case LEFT:
                    return UPLEFT;
            }
            break;
        
        case DOWN:
            switch(secondDirection){
                case RIGHT:
                    return DOWNRIGHT;
                
                case LEFT:
                    return DOWNLEFT;
            }
            break;
    }

    //Error managment
    if(onSameAxis(firstDirection, secondDirection)){
        printf("Direction combination failed : directions are opposite");
    }

    if(isCombinedDirection(firstDirection) || isCombinedDirection(secondDirection)){
        printf("Direction combination failed : one of the directions is already in the combination");
    }

    return firstDirection;
}

Segment::Segment(int row, int col, Direction direction, Segment* next){
    this->row = row;
    this->col = col;
    this->direction = direction;
    this->next = next;
}

Segment::~Segment(){

}

int Segment::GetRow(void){
    return row;
}

int Segment::GetCol(void){
    return col;
}

Direction Segment::GetDirection(void){
    return direction;
}

Segment* Segment::GetNext(void){
    return next;
}

void Segment::SetNext(Segment* nextSegment){
    next = nextSegment;
}

void Segment::SetDirection(Direction newDirection){
    direction = newDirection;
}

//Creates a chained list which represents the snake starting from his tail
Snake::Snake(int startingRow, int startingCol, Direction startingDirection, int length){
    this->length = std::max(2, length);
    this->directionToMove = startingDirection;
    this->crazy = false;

    Segment* segment = NULL;
    //goes from snake's tail to before last segment (last one is the head)
    for(int i=this->length-1;i>0;i--){
        switch (startingDirection){
            case RIGHT:
                segment = new Segment(startingRow, startingCol-i, startingDirection, segment);
                break;
            case LEFT:
                segment = new Segment(startingRow, startingCol+i, startingDirection, segment);
                break;
            case DOWN:
                segment = new Segment(startingRow-i, startingCol, startingDirection, segment);
                break;
            case UP:
                segment = new Segment(startingRow+i, startingCol, startingDirection, segment);
                break;
        }
    }

    head = new Segment(startingRow, startingCol, startingDirection, segment);
}

Snake::~Snake(){

}

//Changes snake's direction by changing head's direction, except if direction is opposite from the current one//
void Snake::ChangeDirection(Direction newDirection){
    if(!onSameAxis(newDirection, directionToMove)){
        directionToMove = newDirection;
    }
}

//Makes the snake go forward in head's direction
//(Game over if collision, eat fruit function)
bool Snake::Move(Playground* playground, Score* score, int* framerate){
    Segment* previousHead = head;

    //Creating new segment in head's direction if movement is leading to Game Over, otherwise we return false and the game ends//
    switch (directionToMove){
        case RIGHT:
            if(head->GetCol() == playground->GetNbCols()-1 || this->occupiesTile(head->GetRow(), head->GetCol()+1)){
                return false;
            }
            head = new Segment(head->GetRow(), head->GetCol()+1, directionToMove, head);
            break;
        case LEFT:
            if(head->GetCol() == 0 || this->occupiesTile(head->GetRow(), head->GetCol()-1)){
                return false;
            }
            head = new Segment(head->GetRow(), head->GetCol()-1, directionToMove, head);
            break;
        case DOWN:
            if(head->GetRow() == playground->GetNbRows()-1 || this->occupiesTile(head->GetRow()+1, head->GetCol())){
                return false;
            }
            head = new Segment(head->GetRow()+1, head->GetCol(), directionToMove, head);
            break;
        case UP:
            if(head->GetRow() == 0 || this->occupiesTile(head->GetRow()-1, head->GetCol())){
                return false;
            }
            head = new Segment(head->GetRow()-1, head->GetCol(), directionToMove, head);
            break;
    }

    /*Makes a combination of previous head's direction and new head's direction before applying it to previous head's direction.
    This combination is used to render snake angle turn sprites, if direction hasn't changed it does nothing*/
    previousHead->SetDirection(CombinedDirection(previousHead->GetDirection(), head->GetDirection()));

    bool snakeIsGrowing = false;
    //If new head collides with a fruit, the snake eats it and grows
    if (head->GetRow() == playground->GetFruit()->GetRow() && head->GetCol() == playground->GetFruit()->GetCol())
    {
        snakeIsGrowing = Eat(playground->GetFruit(), score, framerate);
        playground->SetFruit(NULL);
    }

    /*Before new head creation, snake grows by 1.
    To move need to delete a segment, the easiest one to delete being the tail and the head.
    In this case we chose to delete the tail since we want to keep the head*/
    if (!snakeIsGrowing)
    {
        Segment* actualSegment = head;
        Segment* previousSegment = nullptr;
        while (actualSegment->GetNext() != nullptr){
            previousSegment = actualSegment;
            actualSegment = actualSegment->GetNext();
        }

        if (previousSegment != nullptr){
            previousSegment->SetNext(nullptr);
            delete actualSegment;
        }
    }
    score->update_score(pow(static_cast<double>(length), 2)/(playground->GetNbCols() * playground->GetNbRows()));

    return true;
}

//Eats a fruit, triggers its effect and deletes it from playground
bool Snake::Eat(Fruit* fruitToEat, Score* score, int* framerate){
    bool snakeGrows = false;


    switch(fruitToEat->GetEffect()){
        //Makes snake grow by 1 and adds 1 to fruits eaten in Score
        case BONUS:
            score->update_numb_eaten(1);
            snakeGrows = true;
            length ++;
            break;
            
        //Makes snake shrink by 1
        case SHRINK:
        {
            if(length == 2){
               break; 
            }
            Segment* actualSegment = head;
            Segment* previousSegment = NULL;
            //Checks each segments up until the tail
            while (actualSegment->GetNext() != NULL)
            {
                previousSegment = actualSegment;
                actualSegment = actualSegment->GetNext();
            }
            previousSegment->SetNext(NULL);
            delete actualSegment;
            length --;
            break;
        }

        //Increases snake speed
        case SPEEDUP:
            if(*framerate > 20){
                *framerate -= 5;
            }
            break;
        
        case INVERSE:
            crazy = true;
            break;
    }

    delete fruitToEat;
    return snakeGrows;
}


Segment* Snake::GetHead(){
    return head;
}

//Checks each segment to know if one of them contains coordinates equal to row and col
bool Snake::occupiesTile(int row, int col){
    Segment* actualSegment = head;

    //Check each segment and compares coordinates from each segment with given coordinates
    {
        if (actualSegment->GetCol() == col && actualSegment->GetRow() == row)
        {
            return true;
        }
        
        actualSegment = actualSegment->GetNext();
    }

    return false;
}

void Snake::SetCrazy(bool crazy){
    this->crazy = crazy;
}
        

bool Snake::IsCrazy(){
    return crazy;
}