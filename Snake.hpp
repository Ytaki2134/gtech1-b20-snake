#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <SDL2/SDL.h>

typedef enum{
    DOWN, UP, RIGHT, LEFT
} Direction;

class Segment{
    public :
        Segment(int row, int col, Direction direction, Segment* next);

        ~Segment();

        int GetRow();
        int GetCol();
        Direction GetDirection();
        Segment* GetNext();
        
        void SetNext(Segment* nextSegment);
        void SetDirection(Direction newDirection);

    private :
        int row;
        int col;
        Direction direction;
        Segment* next;

};

class Snake{
    public:
        Snake(int starting_row, int starting_col, Direction starting_direction);

        ~Snake();

        void Move();

        void ChangeDirection(Direction newDirection);

        void draw(SDL_Renderer* mainWindowRenderer, int tile_size);

    private:
        Segment* head;

};


#endif