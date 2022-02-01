#ifndef FRUITS_HPP
#define FRUITS_HPP

#include <SDL2/SDL.h>

typedef enum{
    BONUS, MALUS, SPEEDUP, COUNT
} FruitEffect;

class Fruit{
    public:
        Fruit(int row, int col, FruitEffect effect);
        ~Fruit();

        int GetCol();
        int GetRow();
        FruitEffect GetEffect();

        

    private:
        int row;
        int col;
        FruitEffect effect;
};

#endif
