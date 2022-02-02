#ifndef PLAYGROUNDRENDERER_HPP
#define PLAYGROUNDRENDERER_HPP

#include <SDL2/SDL.h>
#include "fruits.hpp"
#include "snake.hpp"

class PlaygroundRenderer{
    public:
        PlaygroundRenderer();
        ~PlaygroundRenderer();

        void drawGrid();
        void drawFruit(Fruit* fruitToDraw);
        void drawSnake(Snake* snakeToDraw);
        int Init(SDL_Renderer* renderer, SDL_Rect drawZone, int tile_size);
    
    private:
        int InitBackground();

    private:
        SDL_Rect drawZone;
        SDL_Renderer* renderer;
        int tile_size;
        SDL_Texture* bgTexture;
};


#endif