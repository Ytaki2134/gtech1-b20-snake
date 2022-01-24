#include "snake.hpp"
#include "MainSDLWindow.hpp"

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

Snake::Snake(int startingRow, int startingCol, Direction startingDirection, int length){

    Segment* segment = NULL;
    for(int i=length-1;i>0;i--){
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
    int i;
    i+=1;
}

void Snake::ChangeDirection(Direction newDirection){
    Direction current_direction = head->GetDirection();
    if((newDirection == LEFT and current_direction != RIGHT) || (newDirection == RIGHT and current_direction != LEFT)){
        head->SetDirection(newDirection);
    }
    else if((newDirection == UP and current_direction != DOWN) || (newDirection == DOWN and current_direction != UP)){
        head->SetDirection(newDirection);
    }
}

void Snake::Move(){
    Segment* previousHead = head;
    Direction directionToMove = previousHead->GetDirection();
    switch (directionToMove){
        case RIGHT:
            head = new Segment(previousHead->GetRow(), previousHead->GetCol()+1, directionToMove, previousHead);
            break;
        case LEFT:
            head = new Segment(previousHead->GetRow(), previousHead->GetCol()-1, directionToMove, previousHead);
            break;
        case DOWN:
            head = new Segment(previousHead->GetRow()+1, previousHead->GetCol(), directionToMove, previousHead);
            break;
        case UP:
            head = new Segment(previousHead->GetRow()-1, previousHead->GetCol(), directionToMove, previousHead);
            break;
    }

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

void Snake::draw(MainSDLWindow* mainWindow, int tile_size){
    Segment* actual_segment = head;
    while(actual_segment != nullptr)
    {
        SDL_Renderer* mainWindowRenderer = mainWindow->GetRenderer();
        SDL_Rect* playground = mainWindow->GetPlayground();
        SDL_SetRenderDrawColor(mainWindowRenderer, 255, 255, 255, 255);
        SDL_RenderSetViewport(mainWindowRenderer, playground);
        SDL_Rect rectToDraw = {actual_segment->GetCol()*tile_size, actual_segment->GetRow()*tile_size, tile_size, tile_size};
        SDL_RenderDrawRect(mainWindowRenderer, &rectToDraw);
        actual_segment = actual_segment->GetNext();
    }
}