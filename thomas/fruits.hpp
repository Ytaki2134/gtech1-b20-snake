#ifndef FRUITS_HPP
#define FRUITS_HPP

#include <SDL2/SDL.h>

class Fruits {
    public :
        void eaten();
    private :
        int row;
        int col;
};

#endif