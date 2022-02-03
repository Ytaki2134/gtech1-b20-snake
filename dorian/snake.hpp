#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <SDL2/SDL.h>
#include "MainSDLWindow.hpp"
#include "score.hpp"
#include "fruits.hpp"


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

        bool Move(Playground* playground, Score* score, int* framerate);

        void ChangeDirection(Direction newDirection);

        bool Eat(Fruit* fruitToEat, Score* score, int* framerate);

        //bool TestCollisions(Playground* playground);

        //void draw(MainSDLWindow* mainWindow, int tile_size);

        bool occupiesTile(int row, int col);

        Segment* GetHead();
    private:
        Segment* head;

};


#endif