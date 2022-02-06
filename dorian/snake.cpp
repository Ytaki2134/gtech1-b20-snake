#include "snake.hpp"
#include "MainSDLWindow.hpp"
#include "playground.hpp"

bool isCombinedDirection(Direction direction){
    return direction != RIGHT && direction != LEFT && direction != UP && direction != DOWN;
}

bool onSameAxis(Direction firstDirection, Direction secondDirection){
    return (firstDirection == RIGHT && secondDirection == LEFT) 
           || (firstDirection == LEFT && secondDirection == RIGHT)
           || (firstDirection == UP && secondDirection == DOWN)
           || (firstDirection == DOWN && firstDirection == UP);
}

//combine deux directions pour créer une direction qui dont le sens est firstDirection -> secondDirection
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

    //gestion des erreurs
    if(onSameAxis(firstDirection, secondDirection)){
        printf("Erreur combinaison de direction : les deux directions sont opposées");
    }

    if(isCombinedDirection(firstDirection) || isCombinedDirection(secondDirection)){
        printf("Erreur combinaison de direction : une des deux direction est déjà une combinaison");
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

//crée la liste chaînée qui représente le serpent en partant de sa queue
Snake::Snake(int startingRow, int startingCol, Direction startingDirection, int length){
    this->length = length;
    this->directionToMove = startingDirection;

    Segment* segment = NULL;
    // on part de la queue du serpent jusqu'à l'avant dernier segment (le dernier étant la tête)
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

}

//change la direction du serpent en changeant la direction de sa tête, à part si cette direction amenerait le serpent à
//se trourner de 180°
void Snake::ChangeDirection(Direction newDirection){
    if(!onSameAxis(newDirection, directionToMove)){
        directionToMove = newDirection;
    }
}

//fait avancer le serpent dans la direction de la tête et effectue les actions résultantes de ce déplacement
//(game over à cause de colision, manger fruit)
bool Snake::Move(Playground* playground, Score* score, int* framerate){
    Segment* previousHead = head;

    //on va créer un nouveau segment une case plus loin dans la direction de la tête, mais avant de faire cela
    //on vérifie si ce nouveau segment a une position amenant à une défaite (bords du playground ou segment du serpent)
    //si c'est le cas on retourne false pour que la partie se termine.
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

    previousHead->SetDirection(CombinedDirection(previousHead->GetDirection(), head->GetDirection()));

    bool snakeIsGrowing = false;
    // si la nouvelle tête du serpent est sur un fruit, il le mange 
    if (head->GetRow() == playground->GetFruit()->GetRow() && head->GetCol() == playground->GetFruit()->GetCol())
    {
        snakeIsGrowing = Eat(playground->GetFruit(), score, framerate);
        playground->SetFruit(NULL);
    }

    //avec la création de la nouvelle tête, le serpent a grandit de 1. Si on veut seulement se déplacer, il faut alors
    //supprimer un segment quelque part. Les segments les plus facile à supprimer sont la queue et la tête
    //or on ne veut pas supprimer la tête donc on supprime la queue.
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

/* mange un fruit, activant son effet et le faisant disparaître */
bool Snake::Eat(Fruit* fruitToEat, Score* score, int* framerate){
    bool snakeGrows = false;

    switch(fruitToEat->GetEffect()){
        case BONUS:
            score->update_numb_eaten(1);
            snakeGrows = true;
            length ++;
            break;
            
        case SHRINK:
        {
            Segment* actualSegment = head;
            Segment* previousSegment = NULL;
            //parcourir le serpent jusqu'à la queue
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

        case SPEEDUP:
            if(*framerate > 20){
                *framerate -= 5;
            }
            break;
    }

    delete fruitToEat;
    return snakeGrows;
}

/*
void Snake::draw(MainSDLWindow* mainWindow, int tile_size){
    Segment* actual_segment = head;
    while(actual_segment != nullptr)
    {
        SDL_Renderer* mainWindowRenderer = mainWindow->GetRenderer();
        SDL_Rect playground = mainWindow->GetPlaygroundZone();
        SDL_SetRenderDrawColor(mainWindowRenderer, 255, 255, 255, 255);
        SDL_RenderSetViewport(mainWindowRenderer, &playground);
        SDL_Rect rectToDraw = {actual_segment->GetCol()*tile_size, actual_segment->GetRow()*tile_size, tile_size, tile_size};
        SDL_RenderFillRect(mainWindowRenderer, &rectToDraw);
        actual_segment = actual_segment->GetNext();
        SDL_SetRenderDrawColor(mainWindowRenderer, 0, 0, 0, 255);
    }
}
*/

Segment* Snake::GetHead(){
    return head;
}

//parcours le serpent pour savoir si le serpent contient un segment de coordonnées row et col
bool Snake::occupiesTile(int row, int col){
    Segment* actualSegment = head;

    //parcourir tout le serpent et comparer les coordonnées de chaque segment avec les coordonnées données
    while (actualSegment != NULL)
    {
        if (actualSegment->GetCol() == col && actualSegment->GetRow() == row)
        {
            return true;
        }
        
        actualSegment = actualSegment->GetNext();
    }

    return false;
}