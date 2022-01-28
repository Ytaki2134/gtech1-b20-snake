#ifndef FRUITS_HPP
#define FRUITS_HPP

#include <SDL2/SDL.h>

typedef enum{
    BONUS, MALUS, SPEEDUP
} FruitEffect;

class Fruit{
    public:
        Fruit(int row, int col, SDL_Color color, FruitEffect effect);
        ~Fruit();

        

    private:
        int row;
        int col;
        SDL_Color color;
        FruitEffect effect;
};

#endif
