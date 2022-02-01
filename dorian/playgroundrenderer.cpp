
#include "playgroundrenderer.hpp"

PlaygroundRenderer::PlaygroundRenderer(){
    renderer = NULL;
    tile_size = 0;
}

PlaygroundRenderer::~PlaygroundRenderer(){

}

void PlaygroundRenderer::drawGrid(){
    int x = 0;
    int y = 0;

    SDL_RenderSetViewport(renderer, &drawZone);
    SDL_SetRenderDrawColor(renderer, 21, 71, 52, 255);
    
    while(y < drawZone.h){
        if(x >= drawZone.w){
            y += tile_size;
            x = 0;
        }

        SDL_Rect actual_tile = {x, y, tile_size, tile_size};
        SDL_RenderDrawRect(renderer, &actual_tile);

        x += tile_size;
    }

    SDL_RenderSetViewport(renderer, NULL);
}

void PlaygroundRenderer::drawFruit(Fruit* fruitToDraw){

    //couleurs correspondant dans l'ordre des différents effets dans l'enum FruitEffect
    SDL_Color effectColors [3] = {{255, 0, 0, 255}, {189, 183, 107, 255}, {255, 255, 0, 255}}; //red, kaki, yellow
    SDL_RenderSetViewport(renderer, &drawZone);

    //on utilise le fait que les valeurs des effets FruitEffect s'incrémentent à partir de 0 comme les index d'un tableau 
    SDL_SetRenderDrawColor(renderer, effectColors[fruitToDraw->GetEffect()].r, effectColors[fruitToDraw->GetEffect()].g, effectColors[fruitToDraw->GetEffect()].b, effectColors[fruitToDraw->GetEffect()].a);

    SDL_Rect fruitRect = {fruitToDraw->GetCol() * tile_size, fruitToDraw->GetRow() * tile_size, tile_size, tile_size};
    SDL_RenderFillRect(renderer, &fruitRect);
}

int PlaygroundRenderer::Init(SDL_Renderer* renderer, SDL_Rect drawZone, int tile_size){
    this->renderer = renderer;
    this->drawZone = drawZone;
    this->tile_size = tile_size;

    return EXIT_SUCCESS;
}