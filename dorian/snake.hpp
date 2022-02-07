#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <SDL2/SDL.h>
#include "MainSDLWindow.hpp"


typedef enum{
    DOWN, UP, RIGHT, LEFT
} Direction;

class Playground;

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
        Snake(int starting_row, int starting_col, Direction starting_direction, int length);

        ~Snake();

        bool Move();

        void ChangeDirection(Direction newDirection);

        void CheckSegment();

        bool CheckCollision(Playground* playground);

        void draw(MainSDLWindow* mainWindow, int tile_size);

        Segment* GetHead();
    private:
        Segment* head;
        Segment* tail;
        bool Collision;

};


#endif