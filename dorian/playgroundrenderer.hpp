#ifndef PLAYGROUNDRENDERER_HPP
#define PLAYGROUNDRENDERER_HPP

#include <SDL2/SDL.h>
#include "fruits.hpp"
#include "snake.hpp"

class PlaygroundRenderer{
    public:
        PlaygroundRenderer();
        ~PlaygroundRenderer();

        int Init(SDL_Renderer* renderer, SDL_Rect drawZone, Playground* playground);
        void draw(Playground* playground);

    private:
        int InitBackground();
        void drawBackground();
        void drawFruit(Fruit* fruitToDraw);
        void drawSnake(Snake* snakeToDraw);

    private:
        SDL_Rect drawZone;
        Playground* playground;
        SDL_Renderer* renderer;
        SDL_Texture* bgTexture;
        int tileSize;
};


#endif